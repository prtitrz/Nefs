#include "nefs.h"
#include "easyzmq.h"


static int zmqpeer_getattr(const char *path, struct stat *stbuf)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	char type[5];
	sprintf(type, "%d", GETATTR);
	s_sendmore(requester, type);
	
	struct getattr_req req;
	size_t size = sizeof(req);
	memset((char *)&req, 0, size);
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';

	m_send(requester, &req, size);


	struct getattr_res res;
	memset((char *)&res, 0, sizeof(res));
	m_recv(requester, &res);

	if (res.res != 0) {
		zmq_close (requester);
		zmq_term (context);
		return res.res;
	}

	memset((char *)stbuf, 0, sizeof(struct stat));
	memcpy(stbuf, &(res.stbuf), sizeof(struct stat));

	zmq_close (requester);
	zmq_term (context);

    return 0;
}

static int zmqpeer_access(const char *path, int mask)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	char type[5];
	sprintf(type, "%d", ACCESS);
	s_sendmore(requester, type);
	
	struct access_req req;
	size_t size = sizeof(req);
	memset((char *)&req, 0, size);
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';
	req.mask = mask;

	m_send(requester, &req, size);


	struct access_res res;
	memset((char *)&res, 0, sizeof(res));
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_readlink(const char *path, char *buf, size_t size)
{
	int res;

	res = readlink(path, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}


static int zmqpeer_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(path);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int zmqpeer_mknod(const char *path, mode_t mode, dev_t rdev)
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

static int zmqpeer_mkdir(const char *path, mode_t mode)
{
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_unlink(const char *path)
{
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_rmdir(const char *path)
{
	int res;

	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_symlink(const char *from, const char *to)
{
	int res;

	res = symlink(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_rename(const char *from, const char *to)
{
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_link(const char *from, const char *to)
{
	int res;

	res = link(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_chmod(const char *path, mode_t mode)
{
	int res;

	res = chmod(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;

	res = lchown(path, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_truncate(const char *path, off_t size)
{
	int res;

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_utimens(const char *path, const struct timespec ts[2])
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

static int zmqpeer_open(const char *path, struct fuse_file_info *fi)
{
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int zmqpeer_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int zmqpeer_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int zmqpeer_statfs(const char *path, struct statvfs *stbuf)
{
	int res;

	res = statvfs(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int zmqpeer_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) fi;
	return 0;
}

static int zmqpeer_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

static struct fuse_operations zmqpeer_oper = {
	.getattr	= zmqpeer_getattr,
	.access		= zmqpeer_access,
	.readlink	= zmqpeer_readlink,
	.readdir	= zmqpeer_readdir,
	.mknod		= zmqpeer_mknod,
	.mkdir		= zmqpeer_mkdir,
	.symlink	= zmqpeer_symlink,
	.unlink		= zmqpeer_unlink,
	.rmdir		= zmqpeer_rmdir,
	.rename		= zmqpeer_rename,
	.link		= zmqpeer_link,
	.chmod		= zmqpeer_chmod,
	.chown		= zmqpeer_chown,
	.truncate	= zmqpeer_truncate,
	.utimens	= zmqpeer_utimens,
	.open		= zmqpeer_open,
	.read		= zmqpeer_read,
	.write		= zmqpeer_write,
	.statfs		= zmqpeer_statfs,
	.release	= zmqpeer_release,
	.fsync		= zmqpeer_fsync,
};

int main(int argc, char *argv[])
{
	umask(0);

	return fuse_main(argc, argv, &zmqpeer_oper, NULL);
}
