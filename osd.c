#include "xdr.h"
#include "easyzmq.h"
#include "debug.h"

static void read_svc(void *socket, void *controller)
{
	debug_puts("read_svc");
	int fd;

	struct read_req req;
	m_recv(socket, &req);

	struct read_res res;
	fd = open(req.path, O_RDONLY);
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
	fd = open(req.path, O_WRONLY);
	if (fd == -1)
		res.res = -errno;

	res.res = pwrite(fd, req.buf, req.size, req.offset);
	if (res.res == -1)
		res.res = -errno;

	close(fd);

	m_send(controller, &res, sizeof(res));
}

static void init_svc(void *socket, void *controller)
{
	char *string = s_recv(socket);
	free(string);

	s_send(controller, "osd");
	debug_puts("OSD_INIT");
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

	void *receiver = zmq_socket (context, ZMQ_SUB);
	zmq_connect (receiver, "tcp://localhost:5556");
	zmq_setsockopt (receiver, ZMQ_SUBSCRIBE, "", 0);

	sleep(1);
	void *controller = zmq_socket (context, ZMQ_PUSH);
	zmq_bind (controller, "tcp://*:5557");

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
				case READ:
					read_svc(receiver, controller);
					break;
				case WRITE:
					write_svc(receiver, controller);
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
