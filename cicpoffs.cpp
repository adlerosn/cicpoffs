/*
 * Copyright (c) 2020 Adler Neves <adlerosn@gmail.com>
 * Copyright (c) 2023 Bruno Gazotti <bgazotti@gmail.com>
 * 
 * CICPOFFS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 2 of the License.
 */

#include "cicpoffs.hpp"
#include "cicpps.hpp"
extern "C"{
#include "ulockmgr.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <fcntl.h>
#include <sys/time.h>
}

#define VERSION "0.0.1"

static struct fuse_operations operations = {
	.getattr = fuse_fn_getattr,
	.readlink = fuse_fn_readlink,
	// .getdir = <unimplemented> - deprecated,
	.mknod = fuse_fn_mknod,
	.mkdir = fuse_fn_mkdir,
	.unlink = fuse_fn_unlink,
	.rmdir = fuse_fn_rmdir,
	.symlink = fuse_fn_symlink,
	.rename = fuse_fn_rename,
	.link = fuse_fn_link,
	.chmod = fuse_fn_chmod,
	.chown = fuse_fn_chown,
	.truncate = fuse_fn_truncate,
	//.utime = fuse_fn_utime,
	.open = fuse_fn_open,
	.read = fuse_fn_read,
	.write = fuse_fn_write,
	.statfs = fuse_fn_statfs,
	.flush = fuse_fn_flush,
	.release = fuse_fn_release,
	.fsync = fuse_fn_fsync,
	.setxattr = fuse_fn_setxattr,
	.getxattr = fuse_fn_getxattr,
	.listxattr = fuse_fn_listxattr,
	.removexattr = fuse_fn_removexattr,
	.opendir = fuse_fn_opendir,
	.readdir = fuse_fn_readdir,
	.releasedir = fuse_fn_releasedir,
	// .fsyncdir = fuse_fn_fsyncdir,
	.init = fuse_fn_init,
	.destroy = fuse_fn_destroy,
	.access = fuse_fn_access,
	.create = fuse_fn_create,
	//.ftruncate = fuse_fn_ftruncate,
	//.fgetattr = fuse_fn_fgetattr,
	.lock = fuse_fn_lock,
	.utimens = fuse_fn_utimens,
	//.bmap = <unimplemented> - not for block devices,
	//.ioctl = fuse_fn_ioctl,
	//.poll = fuse_fn_poll,
	//.flock = fuse_fn_flock,
	.fallocate = fuse_fn_fallocate,
};

static char* read_source_directory = NULL;

static char* argv0 = NULL;

static void stderr_print(const char* msg){
	fprintf(stderr, "[%s] %s\n", argv0, msg);
};

static void ignore_print(const char* msg){};

static void (*logmsg) (const char* msg) = ignore_print;

void* (fuse_fn_init)        (struct fuse_conn_info* conn, struct fuse_config* cfg){
	struct stat st;
	if(stat(read_source_directory, &st)){
		logmsg("Source does not exist.");
		exit(ENOENT);
	}
	if(read_source_directory[0]!='/'){
		logmsg("Path must be absolute.");
		exit(ENOENT);
	}
	
	return NULL;
};

void  (fuse_fn_destroy)     (void* private_data){
	return;
};

int   (fuse_fn_getattr)     (const char* path, struct stat* stbuf, struct fuse_file_info* fi){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = stat(correctpath, stbuf);
	free((void*) correctpath);
	if(retval==-1) return -errno;
	return retval;
};

int   (fuse_fn_readlink)    (const char* path, char* buf, size_t size){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = readlink(correctpath, buf, size);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_mknod)       (const char* path, mode_t mode, dev_t rdev){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = mknod(correctpath, mode, rdev);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_mkdir)       (const char* path, mode_t mode){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = mkdir(correctpath, mode);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_unlink)      (const char* path){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = unlink(correctpath);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_rmdir)       (const char* path){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = rmdir(correctpath);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_symlink)     (const char* to, const char* from){
	const char* correctfrom = correct_case_sensitivity_for(read_source_directory, from);
	int retval = symlink(to, correctfrom);
	free((void*) correctfrom);
	return retval;
};

// flag behavior is absolutely ignored
int   (fuse_fn_rename)      (const char* from, const char* to, unsigned int flags){
	const char* correctfrom = correct_case_sensitivity_for(read_source_directory, from);
	const char* correctto = correct_case_sensitivity_for(read_source_directory, to);
	int retval = rename(correctfrom, correctto);
	free((void*) correctfrom);
	free((void*) correctto);
	return retval;
};

int   (fuse_fn_link)        (const char* from, const char* to){
	const char* correctfrom = correct_case_sensitivity_for(read_source_directory, from);
	const char* correctto = correct_case_sensitivity_for(read_source_directory, to);
	int retval = link(correctfrom, correctto);
	free((void*) correctfrom);
	free((void*) correctto);
	return retval;
};

int   (fuse_fn_chmod)       (const char* path, mode_t mode, struct fuse_file_info* fi){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = chmod(correctpath, mode);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_chown)       (const char* path, uid_t uid, gid_t gid, struct fuse_file_info*){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = chown(correctpath, uid, gid);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_truncate)    (const char* path, off_t off, struct fuse_file_info*){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = truncate(correctpath, off);
	free((void*) correctpath);
	return retval;
};

/** int   (fuse_fn_utime)       (const char* path, struct utimbuf* buf){
*	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
*	int retval = utime(correctpath, buf);
*	free((void*) correctpath);
*	return retval;
};*/

int   (fuse_fn_open)        (const char* path, struct fuse_file_info* ffi){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int fd = open(correctpath, ffi->flags);
	free((void*) correctpath);
	ffi->fh = fd;
	return (fd==-1) ? -errno : 0;
};

int   (fuse_fn_read)        (const char* path, char* buf, size_t size, off_t off, struct fuse_file_info* ffi){
	return pread(ffi->fh, buf, size, off);
};

int   (fuse_fn_write)       (const char* path, const char* buf, size_t size, off_t off, struct fuse_file_info* ffi){
	return pwrite(ffi->fh, buf, size, off);
};

int   (fuse_fn_statfs)      (const char* path, struct statvfs* svfs){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = statvfs(correctpath, svfs);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_flush)       (const char* path, struct fuse_file_info* ffi){
	return close(dup(ffi->fh));
};

int   (fuse_fn_release)     (const char* path, struct fuse_file_info* ffi){
	return close(ffi->fh);
};

int   (fuse_fn_fsync)       (const char* path, int data_sync, struct fuse_file_info* ffi){
	#ifdef HAVE_FDATASYNC
	if(data_sync)
		return fdatasync(ffi->fh);
	else
	#endif
		return fsync(ffi->fh);
};

int   (fuse_fn_setxattr)    (const char* path, const char* key, const char* value, size_t size, int flags){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = lsetxattr(correctpath, key, value, size, flags);
	free((void*) correctpath);
	if(retval==-1) return -errno;
	return retval;
};

int   (fuse_fn_getxattr)    (const char* path, const char* key, char* value, size_t size){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = lgetxattr(correctpath, key, value, size);
	free((void*) correctpath);
	if(retval==-1) return -errno;
	return retval;
};

int   (fuse_fn_listxattr)   (const char* path, char* list, size_t size){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = llistxattr(correctpath, list, size);
	free((void*) correctpath);
	if(retval==-1) return -errno;
	return retval;
};

int   (fuse_fn_removexattr) (const char* path, const char* key){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = lremovexattr(correctpath, key);
	free((void*) correctpath);
	if(retval==-1) return -errno;
	return retval;
};

int   (fuse_fn_opendir)     (const char* path, struct fuse_file_info* ffi){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	DIR* dp = opendir(correctpath);
	free((void*) correctpath);
	ffi->fh = (uintptr_t) dp;
	return dp==NULL ? -errno : 0;
};

int   (fuse_fn_readdir)     (const char* path, void* buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info* ffi, enum fuse_readdir_flags flags){
	DIR* dp = (DIR*) ffi->fh;
	if(!dp) return -EBADF;
	seekdir(dp, off);
	struct dirent *de;
	while((de = readdir(dp)) != NULL){
		struct stat st;
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, telldir(dp), FUSE_FILL_DIR_PLUS)) break;
	}
	return 0;
};

int   (fuse_fn_releasedir)  (const char* path, struct fuse_file_info* ffi){
	if(ffi->fh)
		closedir((DIR*)(uintptr_t) ffi->fh);
	ffi->fh = 0;
	return 0;
};

// int   (fuse_fn_fsyncdir)    (const char* path, int isdatasync, struct fuse_file_info* ffi){
// 	return ENOSYS;
// };

int   (fuse_fn_access)      (const char* path, int mode){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = access(correctpath, mode);
	free((void*) correctpath);
	return retval;
};

int   (fuse_fn_create)      (const char* path, mode_t mode, struct fuse_file_info* ffi){
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int fd = open(correctpath, ffi->flags, mode);
	free((void*) correctpath);
	if(fd==-1) return -errno;
	ffi->fh = fd;
	return 0;
};

int   (fuse_fn_ftruncate)   (const char* path, off_t off, struct fuse_file_info* ffi){
	return ftruncate(ffi->fh, off);
};

int   (fuse_fn_fgetattr)    (const char* path, struct stat* st, struct fuse_file_info* ffi){
	int retval = fstat(ffi->fh, st);
	if(retval==-1) return -errno;
	return retval;
};

int   (fuse_fn_lock)        (const char* path, struct fuse_file_info* ffi, int cmd, struct flock* lock){
	return ulockmgr_op(ffi->fh, cmd, lock, &ffi->lock_owner, sizeof(ffi->lock_owner));
};

int   (fuse_fn_utimens)     (const char* path, const struct timespec ts[2]){
	struct timeval tv[2];
	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;
	const char* correctpath = correct_case_sensitivity_for(read_source_directory, path);
	int retval = utimes(correctpath, tv);
	free((void*) correctpath);
	return retval;
};

// int   (fuse_fn_ioctl)       (const char* path, int cmd, void* arg, struct fuse_file_info* ffi, unsigned int flags, void* data){
// 	return ENOSYS;
// };

// int   (fuse_fn_poll)        (const char* path, struct fuse_file_info* ffi, struct fuse_pollhandle* ph, unsigned* reventsp){
// 	return ENOSYS;
// };

// int   (fuse_fn_write_buf)   (const char* path, struct fuse_bufvec* buf, off_t off, struct fuse_file_info* ffi){
// 	return ENOSYS;
// };

// int   (fuse_fn_read_buf)    (const char* path, struct fuse_bufvec** buf, size_t size, off_t off, struct fuse_file_info* ffi){
// 	return ENOSYS;
// };

// int   (fuse_fn_flock)       (const char* path, struct fuse_file_info*, int){
// 	return ENOSYS;
// };

int   (fuse_fn_fallocate)   (const char* path, int mode, off_t off, off_t len, struct fuse_file_info* ffi){
	return fallocate(ffi->fh, mode, off, len);
};
