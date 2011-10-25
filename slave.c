#include "slave.h"

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

static void set_path(char* realpath, const char *path)
{
	strcpy(realpath, SLAVE_PATH);
	strncat(realpath, path, PATH_MAX);
}

bool_t truncate_1_svc(ne_truncate_arg arg, ne_truncate_res *res, struct svc_req *req)
{
	char path[PATH_MAX];

	set_path(path, arg.path);
	printf("trun_svc:%s\n", path);

	res->res = truncate(path, arg.size);

	if (res->res == -1) 
		res->res = -errno;

	return TRUE;
}

bool_t read_1_svc(ne_read_arg arg, ne_read_res *res, struct svc_req *req)
{
	int fd;
	char path[PATH_MAX];

	set_path(path, arg.path);
	
	printf("read_svc:%s\n", path);
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

	fd = open(path, O_WRONLY | O_CREAT, RWRWRW);
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
