/**
 *
 * debug.h -- include the infomation when debug 
 *
 * copyright (C) 2011, zfh
 *
 */

#ifndef _DEBUG_H
#define _DEBUG_H

#include <rpc/rpc.h>

#define DEBUG 1//debug switch

#define LOG "/root/log"	//client log:record the error
#define RD_FILE "/root/read"

extern void plog(const char *format, ...);
extern void plog_err(const char *func, const char *info);
extern void plog_entry_location(const char * func, const char *path);
void plog_mode_location(const char * func, const unsigned int size);
extern void plog_exit_status(const char * func, const char *path, int status);
extern void print_rpccall_err(const char *func, enum clnt_stat num);
extern void plog_read(const char *path, char *buf, off_t offset, size_t size);

#ifdef DEBUG
	#undef DPRINT
	#define DPRINT(fmt, args...) fprintf(stderr, fmt, ##args)
	#define print_entry_location() DPRINT("IN:  %s %s:%d\n", __FILE__, __FUNCTION__, __LINE__)
	#define print_exit_location() DPRINT("OUT: %s %s:%d\n", __FILE__, __FUNCTION__, __LINE__)
	#define print_exit_status(stat) DPRINT("OUT: %s %s:%d, STATUS: %d\n", __FILE__, __FUNCTION__, __LINE__, stat)

#else
	#define DPRINT(fmt, args...)
	#define print_entry_location()
	#define print_exit_location()
	#define print_exit_status(stat)
#endif

	#undef IS_ERR
	#define IS_ERR(errno) do{if((errno) < 0) return errno;}while(0)
	#define IS_FALSE(errno) do{if((errno) < 0) return TRUE;}while(0)	//very import return TRUE, represent success, or it will send requst again
	
	#undef PERR
	#define PERR(fmt,args...) {fprintf(stderr, "[%s]:%s:%d:", __FILE__, __FUNCTION__, __LINE__);\
							fprintf(stderr, fmt, ##args);}

#endif

