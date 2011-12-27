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
		res.res = -errno;
	}

	m_send(socket, &res, size);
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
			case MKNOD:
				break;
			case MKDIR:
				break;
			case SYMLINK:
				break;
			case UNLINK:
				break;
			case RMDIR:
				break;
			case RENAME:
				break;
			case LINK:
				break;
			case CHMOD:
				break;
			case CHOWN:
				break;
			case TRUNCATE:
				break;
			case UTIMENS:
				break;
			case OPEN:
				break;
			case READ:
				break;
			case WRITE:
				break;
			case STATFS:
				break;
			default:
				break;
		}


    }
    //  We never get here but if we did, this would be how we end
    zmq_close (responder);
    zmq_term (context);
    return 0;
}
