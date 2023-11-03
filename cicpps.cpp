/* MIT License
 *
 * Copyright (c) 2020 Adler Neves <adlerosn@gmail.com>
 * 
 * CICPOFFS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 2 of the License.
 */

#include "cicpps.hpp"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cwctype>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

namespace fs = std::filesystem;

std::wstring to_wide_string(std::string normal){
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(normal);
}

char* clone_string(const char* source){
    char* cloned = (char*) calloc(strlen(source)+1, sizeof(char));
    strcpy(cloned, source);
    return cloned;
}

const char* recursively_search_case_insensitively(std::string remaining, std::string processed="/"){
    //fprintf(stderr, "[0] %s => %s\n", processed.c_str(), remaining.c_str());
    if(remaining.length()<=0){
        return clone_string(processed.c_str());
    }
    std::string sep = (processed.length()==1) ? "" : "/";
    std::size_t first_slash_pos = remaining.find_first_of('/');
    std::string thispart(remaining.substr(0, first_slash_pos));
    std::string nextRemaining = "";
    if(first_slash_pos != std::string::npos)
        nextRemaining = remaining.substr(first_slash_pos+1);
    //fprintf(stderr, "[1] %s => %s => %s\n", processed.c_str(), thispart.c_str(), nextRemaining.c_str());
    if(!fs::exists(fs::path(processed))){ // if processed does not exist, forward as is
        return clone_string((processed+sep+remaining).c_str());
    } else {
        std::string desiredFragment = processed+sep+thispart;
        if(fs::exists(fs::path(desiredFragment))){ // if case sensivivity here is correct
            return recursively_search_case_insensitively(nextRemaining, desiredFragment);
        } else { // if case sensivivity here is incorrect
            bool found = false;
            std::wstring wpart = to_wide_string(thispart);
            std::transform(wpart.begin(), wpart.end(), wpart.begin(), std::towlower);
            for(const fs::directory_entry &entry : fs::directory_iterator(processed)){
                std::string filename = fs::path(entry.path()).filename();
                //fprintf(stderr, "[2] %s\n", filename.c_str());
                std::wstring wcandidate = to_wide_string(filename);
                std::wstring wcandidatel = wcandidate;
                std::transform(wcandidatel.begin(), wcandidatel.end(), wcandidatel.begin(), std::towlower);
                if(wpart==wcandidatel){
                    //fprintf(stderr, "[3] %s --replacing--> %s\n", filename.c_str(), thispart.c_str());
                    thispart = filename;
                    found = true;
                    break;
                }
            }
            if(!found){
                //fprintf(stderr, "[4] err: %s\n", (processed+sep+remaining).c_str());
                return clone_string((processed+sep+remaining).c_str());
            }else{
                //fprintf(stderr, "[4] scc: %s\n", (processed+sep+thispart).c_str());
                return recursively_search_case_insensitively(nextRemaining, processed+sep+thispart); // remember that we changed 'thispart' in the loop
            }
        }
    }
}

const char* correct_case_sensitivity_for(const char* absolute_mp, const char* relative_cs){
    if(absolute_mp==NULL || relative_cs==NULL) return NULL;
    const char* relative_cs_nls = relative_cs[0]=='/' ? &relative_cs[1] : relative_cs; // remove leading slash
    int absolute_mp_len = strlen(absolute_mp);
    int relative_cs_nls_len = strlen(relative_cs_nls);
    char* full_relative_path = (char*) calloc(absolute_mp_len+relative_cs_nls_len+2, sizeof(char));
    strcpy(full_relative_path, absolute_mp);
    full_relative_path[absolute_mp_len] = '/';
    strcpy(&full_relative_path[absolute_mp_len+1], relative_cs_nls);
    if(fs::exists(fs::path(full_relative_path))){
        return full_relative_path;
    } else {
        const char* full_absolute_fixed_path = recursively_search_case_insensitively(&full_relative_path[1]);
        free((void*) full_relative_path);
        //fprintf(stderr, "[5] scc: %s\n", full_absolute_fixed_path);
        return full_absolute_fixed_path;
    }
}
