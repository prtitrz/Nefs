#include "xdr.h"
#include "easyzmq.h"
#include "debug.h"

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

const char *OSD_PATH = "/home/rz/OSD";

static void set_path(char *realpath, const char *path)
{
	strcpy(realpath, OSD_PATH);
	strncat(realpath, path, PATH_MAX);
}

static void read_svc(void *socket, void *controller)
{
	debug_puts("read_svc");
	int fd;

	struct read_req req;
	m_recv(socket, &req);

	struct read_res res;
	char path[PATH_MAX];

	set_path(path, req.path);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		res.res = -errno;

	res.res = pread(fd, res.buf, req.size, req.offset);
	if (res.res == -1)
		res.res = -errno;

	close(fd);

	m_send(controller, &res, sizeof(res));
}

static void write_svc(void *socket, void *controller)
{
	debug_puts("write_svc");
	int fd;

	struct write_req req;
	m_recv(socket, &req);

	struct write_res res;
	char path[PATH_MAX];

	set_path(path, req.path);
	fd = open(path, O_WRONLY | O_CREAT, RWRWRW);
	if (fd == -1)
		res.res = -errno;

	res.res = pwrite(fd, req.buf, req.size, req.offset);
	if (res.res == -1)
		res.res = -errno;

	close(fd);
	debug_print("%d", res.res);

	m_send(controller, &res, sizeof(res));
}

static void truncate_svc(void *socket, void *controller)
{
	debug_puts("truncate_svc");
	struct truncate_req req;
	m_recv(socket, &req);

	struct truncate_res res;
	char path[PATH_MAX];

	set_path(path, req.path);
	res.res = truncate(path, req.size);
	if (res.res == -1)
		res.res = -errno;

	m_send(controller, &res, sizeof(res));
}

static void init_svc(void *socket, void *controller)
{
	char *string = s_recv(socket);
	free(string);

	s_send(controller, "osd");
	debug_puts("OSD_INIT");
}

static void readsize_svc(void *socket, void *controller)
{
	debug_puts("readlink_svc");
	struct readsize_req req;
	struct readsize_res res;
	m_recv(socket, &req);

	struct stat stbuf;
	char path[PATH_MAX];

	set_path(path, req.path);
	res.res = lstat(path, &stbuf);
	if (res.res == -1) {
		res.res = -errno;
	}

	res.len = stbuf.st_size;

	m_send(controller, &res, sizeof(res));
}

int main(int argc, const char *argv[])
{
	void *context = zmq_init (1);

    //  Socket to talk to clients
    void *responder = zmq_socket (context, ZMQ_REP);
    zmq_bind (responder, "tcp://*:5558");
/*
	void *receiver = zmq_socket (context, ZMQ_PULL);
	zmq_connect (receiver, "tcp://localhost:5560");
	zmq_setsockopt (receiver, ZMQ_SUBSCRIBE, "", 0);

	void *controller = zmq_socket (context, ZMQ_PUSH);
	zmq_bind (controller, "tcp://a:5559");
*/

	void *receiver = zmq_socket (context, ZMQ_PULL);
//	zmq_connect (receiver, "tcp://192.168.0.85:5556");
	zmq_connect (receiver, "tcp://localhost:5556");
//	zmq_setsockopt (receiver, ZMQ_SUBSCRIBE, "", 0);

	sleep(1);
	void *controller = zmq_socket (context, ZMQ_PUSH);
	zmq_connect (controller, "tcp://localhost:5557");

	//Process messages
	zmq_pollitem_t items[] = {
		{ responder, 0, ZMQ_POLLIN, 0 },
		{ receiver, 0, ZMQ_POLLIN, 0 },
	};

    while (1) {
		zmq_poll (items, 2, -1);
		
		if (items [0].revents & ZMQ_POLLIN) {
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
					//init_svc(responder, controller);
					break;
				case READ:
					debug_puts("NO!!");
					//read_svc(responder, controller);
					break;
				case WRITE:
					debug_puts("NO!!");
					//write_svc(responder, controller);
					break;
				default:
					debug_puts("ERROR");
					break;
			}
		}

		if (items [1].revents & ZMQ_POLLIN) {
			char *string = s_recv(receiver);

			int type;
			sscanf(string, "%d", &type);

			free(string);
			int64_t more;
			size_t more_size = sizeof(more);
			zmq_getsockopt(receiver, ZMQ_RCVMORE, &more, &more_size);

			switch(type) {
				case INIT:
					init_svc(receiver, controller);
					break;
				case TRUNCATE:
					truncate_svc(receiver, controller);
					break;
				case READ:
					read_svc(receiver, controller);
					break;
				case WRITE:
					write_svc(receiver, controller);
					break;
				case READSIZE:
					readsize_svc(receiver, controller);
					break;
				default:
					debug_puts("CONTROLLER ERROR");
					break;
			}
		}
	}
    //  We never get here but if we did, this would be how we end
    //zmq_close (responder);
    zmq_close (receiver);
    zmq_close (controller);
    zmq_term (context);
	return 0;
}
