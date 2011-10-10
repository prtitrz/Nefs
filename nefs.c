/*
  Copyright (C) 2011 prtitrz <prtitrz@gmail.com>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.
*/

#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <fuse.h>
#include <fuse_opt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif
#include "nefs.h"
#include "debug.h"
	
char *host = "127.0.0.1";

static int ne_getattr(const char *path, struct stat *stbuf)
{
	CLIENT *clnt;
	static ne_getattr_res res;
	ne_getattr_arg arg;
	int stat;
	
	memset((char *)&res, 0, sizeof(res));

	clnt = clnt_create(host, NEFSPROG, NEFSVERS, "tcp");
	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
		return -errno;
	}
	
	arg.path = strdup(path);
	
	stat = getattr_1(arg, &res, clnt);
	
	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return -errno;
	}
		
	(*stbuf).st_mode = res.stbuf.mode;
	(*stbuf).st_nlink = res.stbuf.nlink;
	(*stbuf).st_size = res.stbuf.size;
	(*stbuf).st_atime = res.stbuf.atime;
	(*stbuf).st_mtime = res.stbuf.mtime;
	(*stbuf).st_ctime = res.stbuf.ctime;

	clnt_destroy(clnt);
	return 0;

}

static int ne_access(const char *path, int mask)
{
	int res;

	res = access(path, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_readlink(const char *path, char *buf, size_t size)
{
	int res;

	res = readlink(path, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}


static int ne_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{

	(void) offset;
	(void) fi;

	CLIENT *clnt;
	static ne_readdir_res res;
	ne_readdir_arg arg;
	ne_dirent *de;
	int stat;

	memset((char *)&res, 0, sizeof(res));

	clnt = clnt_create(host, NEFSPROG, NEFSVERS, "tcp");
	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
		return -errno;
	}
	
	arg.path = strdup(path);
	//FIXME
	//res = (ne_readdir_res *)malloc(sizeof(ne_readdir_res));
	
	stat = readdir_1(arg, &res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return -errno;
	}
	
	for (de = res.dirent; de != NULL; de = de->next) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	clnt_destroy(clnt);

	return 0;
}

static int ne_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_mkdir(const char *path, mode_t mode)
{
	CLIENT *clnt;
	static ne_mkdir_res res;
	ne_mkdir_arg arg;
	int stat;
	
	memset((char *)&res, 0, sizeof(res));

	clnt = clnt_create(host, NEFSPROG, NEFSVERS, "tcp");
	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
		return -errno;
	}
	
	arg.path = strdup(path);
	
	stat = mkdir_1(arg, &res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return -errno;
	}
	
	clnt_destroy(clnt);

	return 0;
}

static int ne_unlink(const char *path)
{
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_rmdir(const char *path)
{
	CLIENT *clnt;
	static ne_rmdir_res res;
	ne_rmdir_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));

	clnt = clnt_create(host, NEFSPROG, NEFSVERS, "tcp");
	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
		return -errno;
	}
	
	arg.path = strdup(path);
	
	stat = rmdir_1(arg, &res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return -errno;
	}
	
	clnt_destroy(clnt);

	return 0;
}

static int ne_symlink(const char *from, const char *to)
{
	int res;

	res = symlink(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_rename(const char *from, const char *to)
{
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_link(const char *from, const char *to)
{
	int res;

	res = link(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_chmod(const char *path, mode_t mode)
{
	int res;

	res = chmod(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;

	res = lchown(path, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_truncate(const char *path, off_t size)
{
	int res;

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(path, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_open(const char *path, struct fuse_file_info *fi)
{
	CLIENT *clnt;
	static ne_open_res res;
	ne_open_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));

	clnt = clnt_create(host, NEFSPROG, NEFSVERS, "tcp");
	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
		return -errno;
	}
	
	arg.path = strdup(path);
	arg.flags = fi->flags;
	
	stat = open_1(arg, &res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return -errno;
	}
	
	clnt_destroy(clnt);

	return 0;

}

static int ne_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	CLIENT *clnt;
	static ne_read_res res;
	ne_read_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));

	clnt = clnt_create(host, NEFSPROG, NEFSVERS, "tcp");
	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
		return -errno;
	}
	
	arg.path = strdup(path);
	arg.size = size;
	arg.offset = offset;
	
	stat = read_1(arg, &res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return -errno;
	}
	
	size = res.res;

	memcpy(buf, res.buf, size);

	clnt_destroy(clnt);

	return size;
}

static int ne_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	CLIENT *clnt;
	static ne_write_res res;
	ne_write_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));

	clnt = clnt_create(host, NEFSPROG, NEFSVERS, "tcp");
	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
		return -errno;
	}
	
	arg.path = strdup(path);
	arg.size = size;
	arg.offset = offset;
	arg.buf = strdup(buf);
	
	stat = write_1(arg, &res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return -errno;
	}
	
	size = res.res;

	clnt_destroy(clnt);

	return size;
}

static int ne_statfs(const char *path, struct statvfs *stbuf)
{
	int res;

	res = statvfs(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int ne_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) fi;
	return 0;
}

static int ne_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

#ifdef HAVE_SETXATTR
/* xattr operations are optional and can safely be left unimplemented */
static int ne_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags)
{
	int res = lsetxattr(path, name, value, size, flags);
	if (res == -1)
		return -errno;
	return 0;
}

static int ne_getxattr(const char *path, const char *name, char *value,
			size_t size)
{
	int res = lgetxattr(path, name, value, size);
	if (res == -1)
		return -errno;
	return res;
}

static int ne_listxattr(const char *path, char *list, size_t size)
{
	int res = llistxattr(path, list, size);
	if (res == -1)
		return -errno;
	return res;
}

static int ne_removexattr(const char *path, const char *name)
{
	int res = lremovexattr(path, name);
	if (res == -1)
		return -errno;
	return 0;
}
#endif /* HAVE_SETXATTR */

static struct fuse_operations ne_oper = {
	.getattr	= ne_getattr,
	.access		= ne_access,
	.readlink	= ne_readlink,
	.readdir	= ne_readdir,
	.mknod		= ne_mknod,
	.mkdir		= ne_mkdir,
	.symlink	= ne_symlink,
	.unlink		= ne_unlink,
	.rmdir		= ne_rmdir,
	.rename		= ne_rename,
	.link		= ne_link,
	.chmod		= ne_chmod,
	.chown		= ne_chown,
	.truncate	= ne_truncate,
	.utimens	= ne_utimens,
	.open		= ne_open,
	.read		= ne_read,
	.write		= ne_write,
	.statfs		= ne_statfs,
	.release	= ne_release,
	.fsync		= ne_fsync,
#ifdef HAVE_SETXATTR
	.setxattr	= ne_setxattr,
	.getxattr	= ne_getxattr,
	.listxattr	= ne_listxattr,
	.removexattr	= ne_removexattr,
#endif
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &ne_oper, NULL);
}
