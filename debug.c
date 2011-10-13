/**
 *
 * log.c -- 
 *
 * copyright (C) 2011, zfh
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>

#include "debug.h"

#ifdef DEBUG

void plog(const char *format, ...)
{
    	va_list args;
    	FILE *logfile = NULL;
    	int must_close = 1, fd;
    
    	va_start(args, format);

#ifdef O_NOFOLLOW
    	if ((fd = open(LOG, O_WRONLY|O_NOFOLLOW|O_APPEND|O_CREAT, 0755)) != -1) {
#else
    	if ((fd = open(LOG, O_WRONLY|O_APPEND|O_CREAT, 0755)) != -1) {
#endif
		if (!(logfile = fdopen(fd,"a"))) {
	   		 logfile = stderr;
	    		must_close = 0;
	    		close(fd);
		}
    	}
	else {
		logfile = stderr;
		must_close = 0;
    	}
    	vfprintf(logfile, format, args);
    	fprintf(logfile, "\n");
   	fflush(logfile);
    	if (must_close)
        	fclose(logfile);

    	va_end(args);
}

void plog_err(const char *func, const char *info)
{
	plog("***error: [%s]:%s", func, info);
}

void print_rpccall_err(const char *func, enum clnt_stat num)
{
	plog("***error: [%s]:%s", func, clnt_sperrno(num));
}

void plog_entry_location(const char * func, const char *path)
{	
	plog("IN: [%s]:%s", func, path);
}

void plog_mode_location(const char * func, const unsigned int mode)
{	
	plog("IN: [%s]:%u", func, mode);
}

void plog_exit_status(const char * func, const char *path, int status)
{
	plog("OUT: [%s]:%s, STATUS: %d", func, path, status);
}

void plog_read(const char *path, char *buf, off_t offset, size_t size)
{
	int fd;
	char rdpath[1024];

	sprintf(rdpath, "%s%s", RD_FILE, path);
	fd = open(rdpath, O_CREAT | O_TRUNC | O_WRONLY, 0755);
	lseek(fd, offset, SEEK_SET);
	write(fd, buf, size);

	close(fd);
}
#else
void plog(const char *format, ...){}
void plog_err(const char *func, const char *info){}
void print_rpccall_err(const char *func, enum clnt_stat num){}
void plog_entry_location(const char * func, const char *path){}
void plog_exit_status(const char * func, const char *path, int status){}
void plog_read(const char *path, char *buf, off_t offset, size_t size){}
#endif
