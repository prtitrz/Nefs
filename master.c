#include "master.h"

static void set_path(char* realpath, const char *path)
{
	strcpy(realpath, MASTER_PATH);
	strncat(realpath, path, PATH_MAX);
}

bool_t getattr_1_svc(ne_getattr_arg arg, ne_getattr_res *res, struct svc_req *req)
{
	char path[PATH_MAX];
	static struct stat stbuf;

	memset((char *)&stbuf, 0, sizeof(stbuf));
	set_path(path, arg.path);

	res->res = lstat(path, &stbuf);

	printf("getattr_svc:%s\n", path);
//	plog_entry_location(__FUNCTION__, arg.path);
	res->stbuf.dev = stbuf.st_dev;
	res->stbuf.ino = stbuf.st_ino;
	res->stbuf.mode = stbuf.st_mode;
//	printf("%u\n", stbuf.st_mode);
	res->stbuf.nlink = stbuf.st_nlink;
	res->stbuf.uid = stbuf.st_uid;
	res->stbuf.gid = stbuf.st_gid;
	res->stbuf.rdev = stbuf.st_rdev;
	res->stbuf.size = stbuf.st_size;
	res->stbuf.atime = stbuf.st_atime;
	res->stbuf.mtime = stbuf.st_mtime;
	res->stbuf.ctime = stbuf.st_ctime;
	res->stbuf.blksize = stbuf.st_blksize;
	res->stbuf.blocks = stbuf.st_blocks;

	if (res->res == -1)
		res->res = -errno;

	return TRUE;
}

bool_t access_1_svc(ne_access_arg arg, ne_access_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);

	res->res = access(path, arg.mask);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t readlink_1_svc(ne_readlink_arg arg, ne_readlink_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);

	res->res = readlink(path, res->buf, arg.size - 1);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t readdir_1_svc(ne_readdir_arg arg, ne_readdir_res *res, struct svc_req *req)
{
	DIR *dp;
	struct dirent *de;
	ne_dirent **d;
	ne_dirent *p;
	char path[PATH_MAX];

	set_path(path, arg.path);

	dp = opendir(path);
	//TODO
	if (dp == NULL) {
		return TRUE;
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
	char path[PATH_MAX];

	set_path(path, arg.path);

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(arg.mode)) {
		res->res = open(path, O_CREAT | O_EXCL | O_WRONLY, arg.mode);
		if (res->res >= 0)
			res->res = close(res->res);
	} else if (S_ISFIFO(arg.mode))
		res->res = mkfifo(path, arg.mode);
	else
		res->res = mknod(path, arg.mode, arg.rdev);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t mkdir_1_svc(ne_mkdir_arg arg, ne_mkdir_res *res, struct svc_req *req)
{
	char path[PATH_MAX];
	//unsigned int temp = 16877;

	set_path(path, arg.path);
	res->res = mkdir(path, arg.mode);
	if (res->res == -1) 
		res->res = -errno;
	
	//printf("%s\n", arg.path);
	//printf("%u\n", (unsigned int)arg.mode);

	return TRUE;
}

bool_t symlink_1_svc(ne_symlink_arg arg, ne_symlink_res *res, struct svc_req *req)
{
	char from[PATH_MAX];
	char to[PATH_MAX];

	set_path(from, arg.from);
	set_path(to, arg.to);

	res->res = symlink(from, to);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t unlink_1_svc(ne_unlink_arg arg, ne_unlink_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);

	res->res = unlink(path);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t rmdir_1_svc(ne_rmdir_arg arg, ne_rmdir_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);

	res->res = rmdir(path);

	if (res->res == -1) 
		res->res = -errno;
	
	return TRUE;
}

bool_t rename_1_svc(ne_rename_arg arg, ne_rename_res *res, struct svc_req *req)
{
	char from[PATH_MAX];
	char to[PATH_MAX];

	set_path(from, arg.from);
	set_path(to, arg.to);

	res->res = rename(from, to);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t link_1_svc(ne_link_arg arg, ne_link_res *res, struct svc_req *req)
{
	char from[PATH_MAX];
	char to[PATH_MAX];

	set_path(from, arg.from);
	set_path(to, arg.to);

	res->res = link(from, to);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t chmod_1_svc(ne_chmod_arg arg, ne_chmod_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);

	res->res = chmod(path, arg.mode);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t chown_1_svc(ne_chown_arg arg, ne_chown_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);

	res->res = chown(path, arg.uid, arg.gid);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t truncate_1_svc(ne_truncate_arg arg, ne_truncate_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);

	res->res = truncate(path, arg.size);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t utimens_1_svc(ne_utimens_arg arg, ne_utimens_res *res, struct svc_req *req)
{
	return TRUE;
}

bool_t open_1_svc(ne_open_arg arg, ne_open_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);

	res->res = open(path, arg.flags);
	//res->res = open("/home/rz/slave/test", arg.flags);
	printf("open_svc:%s\n", path);
	
	close(res->res);

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
