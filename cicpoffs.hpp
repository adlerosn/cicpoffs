/* MIT License
 *
 * Copyright (c) 2020 Adler Neves <adlerosn@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#define FUSE_USE_VERSION 29

#include <fuse.h>

// https://www.cs.hmc.edu/~geoff/classes/hmc.cs135.201001/homework/fuse/fuse_doc.html
void* (fuse_fn_init)        (struct fuse_conn_info*);
void  (fuse_fn_destroy)     (void*);
int   (fuse_fn_getattr)     (const char*, struct stat*);
int   (fuse_fn_readlink)    (const char*, char*, size_t);
int   (fuse_fn_mknod)       (const char*, mode_t, dev_t);
int   (fuse_fn_mkdir)       (const char*, mode_t);
int   (fuse_fn_unlink)      (const char*);
int   (fuse_fn_rmdir)       (const char*);
int   (fuse_fn_symlink)     (const char*, const char*);
int   (fuse_fn_rename)      (const char*, const char*);
int   (fuse_fn_link)        (const char*, const char*);
int   (fuse_fn_chmod)       (const char*, mode_t);
int   (fuse_fn_chown)       (const char*, uid_t, gid_t);
int   (fuse_fn_truncate)    (const char*, off_t);
int   (fuse_fn_utime)       (const char*, struct utimbuf*);
int   (fuse_fn_open)        (const char*, struct fuse_file_info*);
int   (fuse_fn_read)        (const char*, char*, size_t, off_t, struct fuse_file_info*);
int   (fuse_fn_write)       (const char*, const char*, size_t, off_t, struct fuse_file_info*);
int   (fuse_fn_statfs)      (const char*, struct statvfs*);
int   (fuse_fn_flush)       (const char*, struct fuse_file_info*);
int   (fuse_fn_release)     (const char*, struct fuse_file_info*);
int   (fuse_fn_fsync)       (const char*, int, struct fuse_file_info*);
int   (fuse_fn_setxattr)    (const char*, const char*, const char*, size_t, int);
int   (fuse_fn_getxattr)    (const char*, const char*, char*, size_t);
int   (fuse_fn_listxattr)   (const char*, char*, size_t);
int   (fuse_fn_removexattr) (const char*, const char*);
int   (fuse_fn_opendir)     (const char*, struct fuse_file_info*);
int   (fuse_fn_readdir)     (const char*, void*, fuse_fill_dir_t, off_t, struct fuse_file_info*);
int   (fuse_fn_releasedir)  (const char*, struct fuse_file_info*);
int   (fuse_fn_fsyncdir)    (const char*, int, struct fuse_file_info*);
int   (fuse_fn_access)      (const char*, int);
int   (fuse_fn_create)      (const char*, mode_t, struct fuse_file_info*);
int   (fuse_fn_ftruncate)   (const char*, off_t, struct fuse_file_info*);
int   (fuse_fn_fgetattr)    (const char*, struct stat*, struct fuse_file_info*);
int   (fuse_fn_lock)        (const char*, struct fuse_file_info*, int, struct flock*);
int   (fuse_fn_utimens)     (const char*, const struct timespec[2]);
int   (fuse_fn_bmap)        (const char*, size_t, uint64_t*);
int   (fuse_fn_ioctl)       (const char*, int, void*, struct fuse_file_info*, unsigned int, void*);
int   (fuse_fn_poll)        (const char*, struct fuse_file_info*, struct fuse_pollhandle*, unsigned*);
int   (fuse_fn_write_buf)   (const char*, struct fuse_bufvec*, off_t, struct fuse_file_info*);
int   (fuse_fn_read_buf)    (const char*, struct fuse_bufvec**, size_t, off_t, struct fuse_file_info*);
int   (fuse_fn_flock)       (const char*, struct fuse_file_info*, int);
int   (fuse_fn_fallocate)   (const char*, int, off_t, off_t, struct fuse_file_info*);
