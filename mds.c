#include "xdr.h"
#include "easyzmq.h"
#include "debug.h"

const char *MDS_PATH = "/home/rz/MDS";

static void set_path(char *realpath, const char *path)
{
	strcpy(realpath, MDS_PATH);
	strncat(realpath, path, PATH_MAX);
}

static void getattr_svc(void *socket)
{
	debug_puts("attr_svc");
	struct getattr_req req;
	m_recv(socket, &req);

	struct getattr_res res;
	size_t size = sizeof(res);
	memset((char *)&res, 0, size);
	char path[PATH_MAX];

	set_path(path, req.path);
	res.res = lstat(path, &(res.stbuf));
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
	char path[PATH_MAX];

	set_path(path, req.path);
	res.res = access(path, req.mask);
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
	char path[PATH_MAX];

	set_path(path, req.path);
	res.res = readlink(path, res.buf, req.size - 1);

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
	char path[PATH_MAX];

	set_path(path, req.path);

	dp = opendir(path);
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
	char path[PATH_MAX];

	set_path(path, req.path);

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(req.mode)) {
		res.res = open(path, O_CREAT | O_EXCL | O_WRONLY, req.mode);
		if (res.res >= 0) 
			res.res = close(res.res);
	} else if (S_ISFIFO(req.mode)) 
		res.res = mkfifo(path, req.mode);
	else
		res.res = mknod(path, req.mode, req.rdev);
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
	char path[PATH_MAX];

	set_path(path, req.path);

	res.res = mkdir(path, req.mode);
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
	char path[PATH_MAX];

	set_path(path, req.path);

	res.res = unlink(path);
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
	char path[PATH_MAX];

	set_path(path, req.path);

	res.res = rmdir(path);
	if (res.res == -1) {
		res.res = -errno;
	}

	m_send(socket, &res, sizeof(res));
}

static void symlink_svc(void *socket)
{
	debug_puts("symlink_svc");
	struct symlink_req req;
	m_recv(socket, &req);

	struct symlink_res res;
	char from[PATH_MAX];
	char to[PATH_MAX];

	set_path(from, req.from);
	set_path(to, req.to);
	res.res = symlink(from, to);
	if (res.res == -1) 
		res.res = -errno;

	m_send(socket, &res, sizeof(res));
}


static void rename_svc(void *socket)
{
	debug_puts("rename_svc");
	struct rename_req req;
	m_recv(socket, &req);

	struct rename_res res;
	char from[PATH_MAX];
	char to[PATH_MAX];

	set_path(from, req.from);
	set_path(to, req.to);
	res.res = rename(from, to);
	if (res.res == -1) 
		res.res = -errno;

	m_send(socket, &res, sizeof(res));
}

static void link_svc(void *socket)
{
	debug_puts("link_svc");
	struct link_req req;
	m_recv(socket, &req);

	struct link_res res;
	char from[PATH_MAX];
	char to[PATH_MAX];

	set_path(from, req.from);
	set_path(to, req.to);
	res.res = link(from, to);
	if (res.res == -1) 
		res.res = -errno;

	m_send(socket, &res, sizeof(res));
}

static void chmod_svc(void *socket)
{
	debug_puts("chmod_svc");
	struct chmod_req req;
	m_recv(socket, &req);

	struct chmod_res res;
	char path[PATH_MAX];

	set_path(path, req.path);
	res.res = chmod(path, req.mode);
	if (res.res == -1) 
		res.res = -errno;

	m_send(socket, &res, sizeof(res));
}

static void chown_svc(void *socket)
{
	debug_puts("chown_svc");
	struct chown_req req;
	m_recv(socket, &req);

	struct chmod_res res;
	char path[PATH_MAX];

	set_path(path, req.path);
	res.res = lchown(path, req.uid, req.gid);
	if (res.res == -1)
		res.res = -errno;

	m_send(socket, &res, sizeof(res));
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

static void open_svc(void *socket)
{
	debug_puts("open_svc");
	struct open_req req;
	m_recv(socket, &req);

	char path[PATH_MAX];

	set_path(path, req.path);
	struct open_res res;
	res.res = open(path, req.flags);
	if (res.res == -1)
		res.res = -errno;

	close(res.res);

	m_send(socket, &res, sizeof(res));
}

static void read_svc(void *socket, void *controller)
{
	debug_puts("read_svc");
	struct read_req req;
	m_recv(socket, &req);
}

static void write_svc(void *socket, void *controller)
{
	debug_puts("write_svc");

	struct write_req req;
	m_recv(socket, &req);

//	struct write_res res;
/*
	set_path(path, req.path);
	fd = open(path, O_WRONLY);
	if (fd == -1)
		res.res = -errno;

	res.res = pwrite(fd, req.buf, req.size, req.offset);
	if (res.res == -1)
		res.res = -errno;

	close(fd);
	debug_print("%d",res.res);
*/
//	res.res = 5555;
//	debug_print("%d", res.res);
//	m_send(controller, &res, sizeof(res));
}

static void readsize_svc(void *socket, void *controller)
{
	debug_puts("readsize_svc");

	struct readsize_req req;
	m_recv(socket, &req);
}

static void init_svc(void *socket, void *controller)
{
	char *string = s_recv(socket);
	free(string);

	s_send(controller, "mds");
	debug_puts("MDS_INIT");
}

int main (void)
{
    void *context = zmq_init (1);

    //  Socket to talk to clients
    void *responder = zmq_socket (context, ZMQ_REP);
    zmq_bind (responder, "tcp://*:5555");

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
					debug_puts("NO!!!!");
				//	truncate_svc(responder, controller);
					break;
				case OPEN:
					open_svc(responder);
					break;
				case READ:
					break;
				case WRITE:
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
					debug_puts("YES");
					init_svc(receiver, controller);
					break;
				case READ:
					read_svc(receiver, controller);
					break;
				case WRITE:
					write_svc(receiver, controller);
					break;
				case TRUNCATE:
					debug_puts("YES");
					truncate_svc(receiver, controller);
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
    zmq_close (responder);
    zmq_close (receiver);
    zmq_close (controller);
    zmq_term (context);
    return 0;
}
