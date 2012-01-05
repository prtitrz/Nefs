#ifndef _NEFS_H_
#define _NEFS_H


#define FUSE_USE_VERSION 26

/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500

#include <fuse.h>
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <sys/time.h>
#include <errno.h>


enum type { INIT, GETATTR, ACCESS, READLINK, READDIR, MKNOD, MKDIR, SYMLINK, UNLINK, RMDIR, RENAME, LINK, CHMOD, CHOWN, TRUNCATE, UTIMENS, OPEN, READ, WRITE, STATFS };



struct getattr_req {
	char path[PATH_MAX];
};

struct getattr_res {
	struct stat stbuf;
	int res;
};

struct access_req {
	char path[PATH_MAX];
	int mask;
};

struct access_res {
	int res;
};

struct readlink_req {
	char path[PATH_MAX];
	int size;
};

struct readlink_res {
	char buf[4096];
	int res;
	int err;
};

struct readdir_req {
	char path[PATH_MAX];
};

struct readdir_res {
	struct dirent de;
	int res;
	int err;
};

struct mknod_req {
	char path[PATH_MAX];
	unsigned int mode;
	int rdev;
};

struct mknod_res {
	int res;
};

struct mkdir_req {
	char path[PATH_MAX];
	mode_t mode;
};

struct mkdir_res {
	int res;
};

struct unlink_req {
	char path[PATH_MAX];
};

struct unlink_res {
	int res;
};

struct rmdir_req {
	char path[PATH_MAX];
};

struct rmdir_res {
	int res;
};

struct symlink_req {
	char from[PATH_MAX];
	char to[PATH_MAX];
};

struct symlink_res {
	int res;
};

struct rename_req {
	char from[PATH_MAX];
	char to[PATH_MAX];
};

struct rename_res {
	int res;
};

struct link_req {
	char from[PATH_MAX];
	char to[PATH_MAX];
};

struct link_res {
	int res;
};

struct chmod_req {
	char path[PATH_MAX];
	unsigned int mode;
};

struct chmod_res {
	int res;
};

struct chown_req {
	char path[PATH_MAX];
	unsigned int uid;
	unsigned int gid;
};

struct chown_res {
	int res;
};

struct truncate_req {
	char path[PATH_MAX];
	size_t size;
};

struct truncate_res {
	int res;
};

struct utimens_req {};
struct utimens_res {};

struct open_req {
	char path[PATH_MAX];
	int flags;
};

struct open_res {
	int res;
};

struct read_req {
	char path[PATH_MAX];
	int size;
	int offset;
};

struct read_res {
	char buf[4097];
	int res;
};

struct write_req {
	char path[PATH_MAX];
	char buf[4097];
	int size;
	int offset;
};

struct write_res {
	int res;
};

struct statfs_res {};
struct statfs_req {};


#endif
