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
//#include "nefs.h"
#include "debug.h"
#include "client.h"
#include "easyfec.h"

#define K 1
#define M 3
	
char *master = "192.168.0.83";
char *slave[M] = {"192.168.0.84", "127.0.0.1", "192.168.0.83"};
uint64_t test;
int dirty = 0;

static int ne_getattr(const char *path, struct stat *stbuf)
{
	static ne_getattr_res res;
	ne_getattr_arg arg;
	static ne_readsize_res res2;
	ne_readsize_arg arg2;
	int stat;
	
	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	
	stat = cm_getattr(arg, &res, master);
	
	memset((char *)&res2, 0, sizeof(res2));
	arg2.path = strdup(path);
	
	stat = cm_getattr(arg, &res, master);
	stat = cs_readsize(arg2, &res2, slave[0]);
	//TODO:
	//staterr&xdr_free

	if (res.res != 0) {
		return res.res;
	}
	
	(*stbuf).st_ino = res.stbuf.ino;
	(*stbuf).st_mode = res.stbuf.mode;
	(*stbuf).st_nlink = res.stbuf.nlink;
	(*stbuf).st_uid = res.stbuf.uid;
	(*stbuf).st_gid = res.stbuf.gid;
	(*stbuf).st_rdev = res.stbuf.rdev;
//	(*stbuf).st_size = res.stbuf.size;
//	test = res.stbuf.size;

	(*stbuf).st_size = res2.size;
	test = res2.size;

	(*stbuf).st_atime = res.stbuf.atime;
	(*stbuf).st_mtime = res.stbuf.mtime;
	(*stbuf).st_ctime = res.stbuf.ctime;
	(*stbuf).st_blocks = res.stbuf.blocks;
	//(*stbuf).st_dev = res.stbuf.dev;
	//(*stbuf).st_blksize = res.stbuf.blksize;

	(*stbuf).st_dev = 0;
	(*stbuf).st_blksize = 0;

	return 0;
}

static int ne_access(const char *path, int mask)
{
	static ne_access_res res;
	ne_access_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.mask = mask;
	
	stat = cm_access(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_readlink(const char *path, char *buf, size_t size)
{
	static ne_readlink_res res;
	ne_readlink_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.size = size;
	
	stat = cm_readlink(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	res.buf[res.res] = '\0';
	return 0;
}


static int ne_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{

	(void) offset;
	(void) fi;

	static ne_readdir_res res;
	ne_readdir_arg arg;
	ne_dirent *de;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	
	stat = cm_readdir(arg, &res, master);
	//TODO:staterr
	
	for (de = res.dirent; de != NULL; de = de->next) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	//xdrfree();
	return 0;
}

static int ne_mknod(const char *path, mode_t mode, dev_t rdev)
{
	static ne_mknod_res res;
	ne_mknod_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.mode = mode;
	arg.rdev = rdev;
	
	stat = cm_mknod(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_mkdir(const char *path, mode_t mode)
{
	static ne_mkdir_res res;
	ne_mkdir_arg arg;
	int stat;
	
	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.mode = mode;
	
	stat = cm_mkdir(arg, &res, master);
	//TODO:staterr

	//xdr_free();
	return 0;
}

static int ne_unlink(const char *path)
{
	static ne_unlink_res res;
	ne_unlink_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	
	stat = cm_unlink(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_rmdir(const char *path)
{
	static ne_rmdir_res res;
	ne_rmdir_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	
	stat = cm_rmdir(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	return 0;
}

static int ne_symlink(const char *from, const char *to)
{
	static ne_symlink_res res;
	ne_symlink_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.from = strdup(from);
	arg.to = strdup(to);
	
	stat = cm_symlink(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_rename(const char *from, const char *to)
{
	static ne_rename_res res;
	ne_rename_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.from = strdup(from);
	arg.to = strdup(to);
	
	stat = cm_rename(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_link(const char *from, const char *to)
{
	static ne_link_res res;
	ne_link_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.from = strdup(from);
	arg.to = strdup(to);
	
	stat = cm_link(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_chmod(const char *path, mode_t mode)
{
	static ne_chmod_res res;
	ne_chmod_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.mode = mode;
	
	stat = cm_chmod(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_chown(const char *path, uid_t uid, gid_t gid)
{
	static ne_chown_res res;
	ne_chown_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.uid = uid;
	arg.gid = gid;
	
	stat = cm_chown(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_truncate(const char *path, off_t size)
{
	static ne_truncate_res res;
	ne_truncate_arg arg;
	int stat, i;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.size = size;
	
	for (i = 0; i < M; i++) {
		stat = cs_truncate(arg, &res, slave[i]);
	}

	//TODO:
	//xdrfree&staterr

	//FIXME
	if (res.res != 0)
		return res.res;

	return 0;
}

static int ne_utimens(const char *path, const struct timespec ts[2])
{
	//int res;
	//struct timeval tv[2];
	(void) path;

	/* change to a stub
	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(path, tv);
	if (res == -1)
		return -errno;
		*/

	return 0;
}

static int ne_open(const char *path, struct fuse_file_info *fi)
{
	static ne_open_res res;
	ne_open_arg arg;
	int stat;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.flags = fi->flags;
	
	stat = cm_open(arg, &res, master);

	//TODO:
	//staterr&xdrfree

	return 0;
}

static int ne_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	static ne_read_res res;
	ne_read_arg arg;
	static ne_readsize_res res2;
	ne_readsize_arg arg2;
	(void) fi;
	int stat, i, j, miss, l;
	char *data;
	char *output;
	unsigned index[K];
	int temp[M];

	//test = test - 1;


	arg2.path = strdup(path);
	memset((char *)&res2, 0, sizeof(res2));
	
	for (i = 0; i < M; i++) {
		stat = cs_readsize(arg2, &res2, slave[i]);
		if (res2.res == 0) 
			break;
	}

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	//size = res2.size;

	data = (char *)malloc((M * res2.size + 1) * sizeof(char));
	arg.path = strdup(path);
	arg.size = res2.size * K;
	arg.offset = offset;
	

	for (i = 0; i < M; i++) {
		stat = cs_read(arg, &res, slave[i]);
		temp[i] = -stat;
		if (!stat) {
	//		temp[i] = i;
			for (j = 0; j < res2.size; j++) {
				data[i * res2.size + j] = res.buf[j];
			}
		}
		else {
	//		temp[i] = -1;
			for (j = 0; j < res2.size; j++) {
				data[i * res2.size + j] = 'x';
			}		
		}
	}
	data[M * res2.size] = '\0';
	index[0] = 1;
	/*
	for (i = 0; i < K; i++) {
		if (temp[i] != i)
		{
			//FIXME
			for (j = l; j < M; j++) {
				if (temp[j] == j) {
					index[i] = j;
					l = j + 1;
					break;
				}
			}
		}
		else {
			index[i] = i;
		}
	}
	*/


	//TODO:
	//staterr&xdrfree
//	miss = cal_miss(index, K);
	
	output = decode(data, K, M, index, 1, res2.size);
	size = res.res;
	memcpy(buf, output, size);
	//buf = strdup(res.buf);
	//buf[size] = '\0';

	return size;
}

static int ne_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	static ne_write_res res;
	ne_write_arg arg;
	int stat, i, chunksize;
	char *output;
	(void) fi;

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
//	arg.size = size;
	arg.offset = offset;
//	arg.buf = strdup(buf);

	chunksize = div_ceil(size, K);
	arg.size = chunksize;
	output = encode(buf, K, M, chunksize);
	

	for (i = 0; i < M; i++) {
		arg.buf = strndup(output + (i * chunksize), chunksize);
		stat = cs_write(arg, &res, slave[i]);
	}

	//TODO:
	//staterr&xdrfree
	
//	size = res.res;
	//test = size;
	free(output);

	dirty = 1;

	return size;
}

static int ne_statfs(const char *path, struct statvfs *stbuf)
{
//	int res;
	(void) path;
	(void) stbuf;

	/* change to stub
	res = statvfs(path, stbuf);
	if (res == -1)
		return -errno;
	*/
	return 0;
}

static int ne_release(const char *path, struct fuse_file_info *fi)
{
	static ne_truncate_res res;
	ne_truncate_arg arg;
	static ne_readsize_res res2;
	ne_readsize_arg arg2;
	int stat, i;

/*
	arg2.path = strdup(path);
	memset((char *)&res2, 0, sizeof(res2));
	
	for (i = 0; i < M; i++) {
		stat = cs_readsize(arg2, &res2, slave[i]);
		if (res2.res == 0) 
			break;
	}

	memset((char *)&res, 0, sizeof(res));
	arg.path = strdup(path);
	arg.size = res2.size * K;

	
	stat = cm_truncate(arg, &res, master);

	//TODO:
	//xdrfree&staterr

	if (res.res != 0)
		return res.res;
*/
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
