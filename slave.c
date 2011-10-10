#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif
#include "nefs.h"
#include "debug.h"

bool_t getattr_1_svc(ne_getattr_arg arg, ne_getattr_res *res, struct svc_req *req)
{
	static struct stat stbuf;
	memset((char *)&stbuf, 0, sizeof(stbuf));
	res->res = lstat(arg.path, &stbuf);
	if (res->res == -1)
		return TRUE;
	
	res->stbuf.mode = stbuf.st_mode;
	res->stbuf.nlink = stbuf.st_nlink;
	res->stbuf.size = stbuf.st_size;
	res->stbuf.atime = stbuf.st_atime;
	res->stbuf.mtime = stbuf.st_mtime;
	res->stbuf.ctime = stbuf.st_ctime;

	return TRUE;
}

bool_t access_1_svc(ne_access_arg arg, ne_access_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t readlink_1_svc(ne_readlink_arg arg, ne_readlink_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t readdir_1_svc(ne_readdir_arg arg, ne_readdir_res *res, struct svc_req *req)
{
	DIR *dp;
	struct dirent *de;
	ne_dirent **d;
	ne_dirent *p;

	dp = opendir(arg.path);
	//TODO
	if (dp == NULL) {
		return FALSE;
	}

	d = &(res->dirent);

	while ((de = readdir(dp)) != NULL) {
		p = *d = (ne_dirent *)malloc(sizeof(ne_dirent));
		//TODO:memory full
		p->d_ino = de->d_ino;
		p->d_name = strdup(de->d_name);
		d = &p->next;
	}
	*d = (ne_dirent *)NULL;

	closedir(dp);
	return TRUE;
}

bool_t mknod_1_svc(ne_mknod_arg arg, ne_mknod_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t mkdir_1_svc(ne_mkdir_arg arg, ne_mkdir_res *res, struct svc_req *req)
{
	res->res = mkdir(arg.path, arg.mode);
	if (res->res == -1) {
		return FALSE;
	}
	return TRUE;
}

bool_t symlink_1_svc(ne_symlink_arg arg, ne_symlink_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t unlink_1_svc(ne_unlink_arg arg, ne_unlink_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t rmdir_1_svc(ne_rmdir_arg arg, ne_rmdir_res *res, struct svc_req *req)
{
	res->res = rmdir(arg.path);
	if (res->res == -1) {
		return FALSE;
	}
	return TRUE;
}

bool_t rename_1_svc(ne_rename_arg arg, ne_rename_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t link_1_svc(ne_link_arg arg, ne_link_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t chmod_1_svc(ne_chmod_arg arg, ne_chmod_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t chown_1_svc(ne_chown_arg arg, ne_chown_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t truncate_1_svc(ne_truncate_arg arg, ne_truncate_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t utimens_1_svc(ne_utimens_arg arg, ne_utimens_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t open_1_svc(ne_open_arg arg, ne_open_res *res, struct svc_req *req)
{
	res->res = open(arg.path, arg.flags);
	if (res->res == -1) {
		return FALSE;
	}
	close(res->res);
	return TRUE;
}

bool_t read_1_svc(ne_read_arg arg, ne_read_res *res, struct svc_req *req)
{
	int fd;

	fd = open(arg.path, O_RDONLY);
	if (fd == -1) {
		return FALSE;
	}
	//FIXME
	res->buf = (char *)malloc(arg.size * sizeof(char));

	res->res = pread(fd, res->buf, arg.size, arg.offset);

	if (res->res == -1) {
		return FALSE;
	}
	close(fd);
	return TRUE;
}

bool_t write_1_svc(ne_write_arg arg, ne_write_res *res, struct svc_req *req)
{
	int fd;
	
	fd = open(arg.path, O_WRONLY);
	if (fd == -1) {
		return FALSE;
	}
	res->res = pwrite(fd, arg.buf, arg.size, arg.offset);

	if (res->res == -1) {
		return FALSE;
	}
	close(fd);
	return TRUE;
}

bool_t statfs_1_svc(ne_statfs_arg arg, ne_statfs_res *res, struct svc_req *req)
{
	return TRUE;
}

//FIXME
int nefsprog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free(xdr_result, result);
	return TRUE;
}
