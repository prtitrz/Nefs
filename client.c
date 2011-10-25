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

int cm_access(ne_access_arg arg, ne_access_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = access_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_readlink(ne_readlink_arg arg, ne_readlink_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = readlink_1(arg, res, clnt);

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

int cm_mknod(ne_mknod_arg arg, ne_mknod_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = mknod_1(arg, res, clnt);

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

int cm_unlink(ne_unlink_arg arg, ne_unlink_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = unlink_1(arg, res, clnt);

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

int cm_symlink(ne_symlink_arg arg, ne_symlink_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = symlink_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_rename(ne_rename_arg arg, ne_rename_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = rename_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_link(ne_link_arg arg, ne_link_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = link_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_chmod(ne_chmod_arg arg, ne_chmod_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = chmod_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cm_chown(ne_chown_arg arg, ne_chown_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = chown_1(arg, res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	clnt_destroy(clnt);

	return 0;
}

int cs_truncate(ne_truncate_arg arg, ne_truncate_res *res, char *ip)
{
	CLIENT *clnt;
	int stat;

	clnt = clnt_create(ip, CSPROG, CSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = truncate_1(arg, res, clnt);

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

	clnt = clnt_create(ip, CSPROG, CSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = read_1(arg, res, clnt);
	plog_entry_location(__FUNCTION__, res->buf);

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

	clnt = clnt_create(ip, CSPROG, CSVERS, "tcp");

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

