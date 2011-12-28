#include "nefs.h"
#include "easyzmq.h"
#include "debug.h"


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
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("readlink_loc");

	t_sendmore(requester, READLINK);
	
	struct readlink_req req;
	size_t len = sizeof(req);
	memset((char *)&req, 0, len);
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';
	req.size = size;

	m_send(requester, &req, len);


	struct readlink_res res;
	memset((char *)&res, 0, sizeof(res));
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res == -1) {
		return res.err;
	}

	memcpy(buf, &(res.buf), res.res + 1);
	buf[res.res] = '\0';
	debug_puts(buf);

	return 0;
}


static int zmqpeer_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("readdir_loc");

	t_sendmore(requester, READDIR);
	
	struct readdir_req req;
	size_t len = sizeof(req);
	memset((char *)&req, 0, len);
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';

	m_send(requester, &req, len);

	struct readdir_res res;

	struct stat st;

	while (1) {
		m_recv(requester, &res);
		if (res.err != 0) 
			return res.err;
		if (res.res == -1) 
			break;
		int64_t more;
		size_t more_size = sizeof(more);
		zmq_getsockopt(requester, ZMQ_RCVMORE, &more, &more_size);
		memset(&st, 0, sizeof(st));
		st.st_ino = res.de.d_ino;
		st.st_mode = res.de.d_type << 12;
		if (filler(buf, res.de.d_name, &st, 0)) 
			break;
	}


	zmq_close (requester);
	zmq_term (context);

	return 0;
}

static int zmqpeer_mknod(const char *path, mode_t mode, dev_t rdev)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("mknod_loc");

	t_sendmore(requester, MKNOD);
	
	struct mknod_req req;
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';
	req.mode = mode;
	req.rdev = rdev;

	m_send(requester, &req, sizeof(req));

	struct mknod_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_mkdir(const char *path, mode_t mode)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("mkdir_loc");

	t_sendmore(requester, MKDIR);
	
	struct mkdir_req req;
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';
	req.mode = mode;

	m_send(requester, &req, sizeof(req));

	struct mkdir_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_unlink(const char *path)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("unlink_loc");

	t_sendmore(requester, MKDIR);
	
	struct unlink_req req;
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';

	m_send(requester, &req, sizeof(req));

	struct unlink_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_rmdir(const char *path)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("rmdir_loc");

	t_sendmore(requester, RMDIR);
	
	struct rmdir_req req;
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';

	m_send(requester, &req, sizeof(req));

	struct rmdir_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_symlink(const char *from, const char *to)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("symlink_loc");

	t_sendmore(requester, SYMLINK);
	
	struct symlink_req req;
	strncpy(req.from, from, strlen(from));
	req.from[strlen(from)] = '\0';
	strncpy(req.to, to, strlen(to));
	req.to[strlen(to)] = '\0';

	m_send(requester, &req, sizeof(req));

	struct symlink_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_rename(const char *from, const char *to)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("rename_loc");

	t_sendmore(requester, RENAME);
	
	struct rename_req req;
	strncpy(req.from, from, strlen(from));
	req.from[strlen(from)] = '\0';
	strncpy(req.to, to, strlen(to));
	req.to[strlen(to)] = '\0';

	m_send(requester, &req, sizeof(req));

	struct rename_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_link(const char *from, const char *to)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	debug_puts("link_loc");

	t_sendmore(requester, LINK);
	
	struct link_req req;
	strncpy(req.from, from, strlen(from));
	req.from[strlen(from)] = '\0';
	strncpy(req.to, to, strlen(to));
	req.to[strlen(to)] = '\0';

	m_send(requester, &req, sizeof(req));

	struct link_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_chmod(const char *path, mode_t mode)
{
	debug_puts("chmod_loc, this is not allowed");

	/*Not allowed to chmod
	 *
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	t_sendmore(requester, CHMOD);
	
	struct chmod_req req;
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';
	req.mode = mode;

	m_send(requester, &req, sizeof(req));

	struct chmod_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}
	*/
	return 0;
}

static int zmqpeer_chown(const char *path, uid_t uid, gid_t gid)
{
	debug_puts("chown_loc, this is not allowed");

	/*Not allowed to chown
	 *
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

	t_sendmore(requester, CHOWN);
	
	struct chown_req req;
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';
	req.uid = uid;
	req.gid = gid;

	m_send(requester, &req, sizeof(req));

	struct chown_res res;
	m_recv(requester, &res);

	zmq_close (requester);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}
	*/
	return 0;
}

static int zmqpeer_truncate(const char *path, off_t size)
{
	void *context = zmq_init (1);

    //  Socket to talk to server
    void *sender = zmq_socket (context, ZMQ_PUB);
    zmq_bind (sender, "tcp://*:5556");

	void *controller = zmq_socket(context, ZMQ_PULL);
	zmq_connect (controller, "tcp://localhost:5557");

	t_sendmore(sender, TRUNCATE);
	
	struct truncate_req req;
	strncpy(req.path, path, strlen(path));
	req.path[strlen(path)] = '\0';
	req.size = size;

	m_send(sender, &req, sizeof(req));

	struct truncate_res res;
	m_recv(sender, &res);

	zmq_close (sender);
	zmq_term (context);

	if (res.res != 0) {
		return res.res;
	}

	return 0;
}

static int zmqpeer_utimens(const char *path, const struct timespec ts[2])
{
	debug_puts("utimens_loc, this is not supported");

	/*Not support utimens
	 *
	int res;
	struct timeval tv[2];

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
	debug_puts("statfs_loc, this is not supported");

	/*Not support statfs
	 *
	int res;

	res = statvfs(path, stbuf);
	if (res == -1)
		return -errno;
	*/

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
