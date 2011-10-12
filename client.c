#include <errno.h>
#include "client.h"
#include "debug.h"

int cm_getattr(ne_getattr_arg arg, ne_getattr_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = getattr_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_readdir(ne_readdir_arg arg, ne_readdir_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = readdir_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_mkdir(ne_mkdir_arg arg, ne_mkdir_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = mkdir_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_rmdir(ne_rmdir_arg arg, ne_rmdir_res *res, char *ip)
{	
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = rmdir_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_open(ne_open_arg arg, ne_open_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = open_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cs_read(ne_read_arg arg, ne_read_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = read_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cs_write(ne_write_arg arg, ne_write_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = write_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

