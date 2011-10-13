#include "slave.h"

static void set_path(char* realpath, const char *path)
{
	strcpy(realpath, SLAVE_PATH);
	strncat(realpath, path, PATH_MAX);
}

bool_t read_1_svc(ne_read_arg arg, ne_read_res *res, struct svc_req *req)
{
	int fd;
	char path[PATH_MAX];

	set_path(path, arg.path);

	fd = open(path, O_RDONLY);
	if (fd == -1) {
		return FALSE;
	}
	//FIXME
	res->buf = (char *)malloc(arg.size * sizeof(char));

	res->res = pread(fd, res->buf, arg.size, arg.offset);
	printf("%s\t%s", path, res->buf);

	close(fd);

	return TRUE;
}

bool_t write_1_svc(ne_write_arg arg, ne_write_res *res, struct svc_req *req)
{
	int fd;
	char path[PATH_MAX];

	set_path(path, arg.path);

	printf("write_svc:%s\n", path);

	fd = open(path, O_WRONLY);
	if (fd == -1) {
		return FALSE;
	}
	res->res = pwrite(fd, arg.buf, arg.size, arg.offset);
	printf("%s\t%s\n", path, arg.buf);

	close(fd);

	return TRUE;
}

//FIXME
int csprog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free(xdr_result, result);
	return TRUE;
}
