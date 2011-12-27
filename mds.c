#include "nefs.h"
#include "easyzmq.h"

static void getattr_svc(void *socket)
{
	struct getattr_req req;
	m_recv(socket, &req);

	struct getattr_res res;
	size_t size = sizeof(res);
	memset((char *)&res, 0, size);
	res.res = lstat(req.path, &(res.stbuf));
	if (res.res == -1) {
		res.res = -errno;
	}

	m_send(socket, &res, size);
}

static void access_svc(void *socket)
{
	struct access_req req;
	m_recv(socket, &req);

	struct access_res res;
	size_t size = sizeof(res);
	memset((char *)&res, 0, size);
	res.res = access(req.path, req.mask);
	if (res.res == -1) {
		res.res = -errno;
	}

	m_send(socket, &res, size);
}

static void readlink_svc(void *socket)
{
	struct readlink_req req;
	m_recv(socket, &req);

	struct readlink_res res;
	size_t size = sizeof(res);
	memset((char *)&res, 0, size);
	res.res = readlink(req.path, res.buf, req.size - 1);

	if (res.res == -1) {
		res.err = -errno;
	}

	m_send(socket, &res, size);
}

static void readdir_svc(void *socket)
{
	struct readdir_req req;
	m_recv(socket, &req);

	struct readdir_res res;
	size_t size = sizeof(res);
	memset((char *)&res, 0, size);

	DIR *dp;
	struct dirent *de;

	dp = opendir(req.path);
	res.err = 0;
	if (dp == NULL) {
		res.err = -errno;
		m_send(socket, &res, size);
		return;
	}

	while ((de = readdir(dp)) != NULL) {
		size = sizeof(struct dirent);
		memcpy(&(res.de), de, size);
		res.res = 0;
		m_sendmore(socket, &res, sizeof(res));
	}
	//FIXME:Ugly solution to end more
	res.res = -1;
	m_send(socket, &res, sizeof(res));
	closedir(dp);
}

static void mknod_svc(void *socket)
{
	debug_puts("mknod_svc");
	struct mknod_req req;
	m_recv(socket, &req);

	struct mknod_res res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(req.mode)) {
		res.res = open(req.path, O_CREAT | O_EXCL | O_WRONLY, req.mode);
		if (res.res >= 0) 
			res.res = close(res.res);
	} else if (S_ISFIFO(req.mode)) 
		res.res = mkfifo(req.path, req.mode);
	else
		res.res = mknod(req.path, req.mode, req.rdev);
	if (res.res == -1) 
		res.res = -errno;

	m_send(socket, &res, sizeof(res));
}

static void mkdir_svc(void *socket)
{
	debug_puts("mkdir_svc");
	struct mkdir_req req;
	m_recv(socket, &req);

	struct mkdir_res res;

	res.res = mkdir(req.path, req.mode);
	if (res.res == -1) 
		res.res = -errno;

	m_send(socket, &res, sizeof(res));
}

static void unlink_svc(void *socket)
{
	debug_puts("unlink_svc");
	struct unlink_req req;
	m_recv(socket, &req);

	struct unlink_res res;

	res.res = unlink(req.path);
	if (res.res == -1) 
		res.res = -errno;

	m_send(socket, &res, sizeof(res));
}

static void rmdir_svc(void *socket)
{
	debug_puts("rmdir_svc");
	struct rmdir_req req;
	m_recv(socket, &req);

	struct rmdir_res res;

	res.res = rmdir(req.path);
	if (res.res == -1) {
		res.res = -errno;
	}

	m_send(socket, &res, sizeof(res));
}

int main (void)
{
    void *context = zmq_init (1);

    //  Socket to talk to clients
    void *responder = zmq_socket (context, ZMQ_REP);
    zmq_bind (responder, "tcp://*:5555");

    while (1) {
        //  Wait for next request from client
		char *string = s_recv(responder);

		int type;
		sscanf(string, "%d", &type);

		free(string);
		int64_t more;
		size_t more_size = sizeof(more);
		zmq_getsockopt(responder, ZMQ_RCVMORE, &more, &more_size);

		switch(type) {
			case INIT:
				break;
			case GETATTR:
				getattr_svc(responder);
				break;
			case ACCESS:
				access_svc(responder);
				break;
			case READLINK:
				readlink_svc(responder);
				break;
			case READDIR:
				readdir_svc(responder);
				break;
			case MKNOD:
				mknod_svc(responder);
				break;
			case MKDIR:
				mkdir_svc(responder);
				break;
			case SYMLINK:
				symlink_svc(responder);
				break;
			case UNLINK:
				unlink_svc(responder);
				break;
			case RMDIR:
				rmdir_svc(responder);
				break;
			case RENAME:
				rename_svc(responder);
				break;
			case LINK:
				link_svc(responder);
				break;
			case CHMOD:
				chmod_svc(responder);
				break;
			case CHOWN:
				chown_svc(responder);
				break;
			case TRUNCATE:
				truncate_svc(responder);
				break;
			case UTIMENS:
				utimens_svc(responder);
				break;
			case OPEN:
				open_svc(responder);
				break;
			case READ:
				break;
			case WRITE:
				break;
			case STATFS:
				statfs_svc(responder);
				break;
			default:
				debug_puts("ERROR");
				break;
		}


    }
    //  We never get here but if we did, this would be how we end
    zmq_close (responder);
    zmq_term (context);
    return 0;
}
