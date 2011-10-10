#include "client.h"
#include "debug.h"

int cm_readdir_1(ne_readdir_arg arg, ne_readdir_res *res, char *ip)
{
	CLIENT *clnt;

	clnt = clnt_create(ip, NEFSPROG, NEFSVERS, "tcp");

	//TODO:clnt == NULL
	if (clnt == NULL) {
		plog_entry_location(__FUNCTION__, "clnt NULL");
	}

	stat = readdir_1(arg, &res, clnt);

	//TODO:
	if (stat != RPC_SUCCESS) {
		print_rpccall_err(__FUNCTION__, stat);
		return 1;
	}

	return 0;
}

