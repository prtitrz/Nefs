/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _NEFS_H_RPCGEN
#define _NEFS_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


struct nefs_stat {
	uint64_t dev;
	uint64_t ino;
	u_int mode;
	uint32_t nlink;
	u_int uid;
	u_int gid;
	uint64_t rdev;
	uint64_t size;
	uint64_t atime;
	uint64_t mtime;
	uint64_t ctime;
	uint64_t blksize;
	uint64_t blocks;
};
typedef struct nefs_stat nefs_stat;

struct ne_dirent {
	uint64_t d_ino;
	char *d_name;
	struct ne_dirent *next;
};
typedef struct ne_dirent ne_dirent;

struct ne_getattr_arg {
	char *path;
};
typedef struct ne_getattr_arg ne_getattr_arg;

struct ne_getattr_res {
	struct nefs_stat stbuf;
	int res;
};
typedef struct ne_getattr_res ne_getattr_res;

struct ne_access_arg {
	char *path;
	int mask;
};
typedef struct ne_access_arg ne_access_arg;

struct ne_access_res {
	int res;
};
typedef struct ne_access_res ne_access_res;

struct ne_readlink_arg {
	char *path;
	uint64_t size;
};
typedef struct ne_readlink_arg ne_readlink_arg;

struct ne_readlink_res {
	char *buf;
	int res;
};
typedef struct ne_readlink_res ne_readlink_res;

struct ne_readdir_arg {
	char *path;
};
typedef struct ne_readdir_arg ne_readdir_arg;

struct ne_readdir_res {
	ne_dirent *dirent;
};
typedef struct ne_readdir_res ne_readdir_res;

struct ne_mknod_arg {
	char *path;
	u_int mode;
	uint64_t rdev;
};
typedef struct ne_mknod_arg ne_mknod_arg;

struct ne_mknod_res {
	int res;
};
typedef struct ne_mknod_res ne_mknod_res;

struct ne_mkdir_arg {
	char *path;
	u_int mode;
};
typedef struct ne_mkdir_arg ne_mkdir_arg;

struct ne_mkdir_res {
	int res;
};
typedef struct ne_mkdir_res ne_mkdir_res;

struct ne_unlink_arg {
	char *path;
};
typedef struct ne_unlink_arg ne_unlink_arg;

struct ne_unlink_res {
	int res;
};
typedef struct ne_unlink_res ne_unlink_res;

struct ne_rmdir_arg {
	char *path;
};
typedef struct ne_rmdir_arg ne_rmdir_arg;

struct ne_rmdir_res {
	int res;
};
typedef struct ne_rmdir_res ne_rmdir_res;

struct ne_symlink_arg {
	char *from;
	char *to;
};
typedef struct ne_symlink_arg ne_symlink_arg;

struct ne_symlink_res {
	int res;
};
typedef struct ne_symlink_res ne_symlink_res;

struct ne_rename_arg {
	char *from;
	char *to;
};
typedef struct ne_rename_arg ne_rename_arg;

struct ne_rename_res {
	int res;
};
typedef struct ne_rename_res ne_rename_res;

struct ne_link_arg {
	char *from;
	char *to;
};
typedef struct ne_link_arg ne_link_arg;

struct ne_link_res {
	int res;
};
typedef struct ne_link_res ne_link_res;

struct ne_chmod_arg {
	char *path;
	u_int mode;
};
typedef struct ne_chmod_arg ne_chmod_arg;

struct ne_chmod_res {
	int res;
};
typedef struct ne_chmod_res ne_chmod_res;

struct ne_chown_arg {
	char *path;
	u_int uid;
	u_int gid;
};
typedef struct ne_chown_arg ne_chown_arg;

struct ne_chown_res {
	int res;
};
typedef struct ne_chown_res ne_chown_res;

struct ne_truncate_arg {
	char *path;
	uint64_t size;
};
typedef struct ne_truncate_arg ne_truncate_arg;

struct ne_truncate_res {
	int res;
};
typedef struct ne_truncate_res ne_truncate_res;

struct ne_utimens_arg {
	int res;
};
typedef struct ne_utimens_arg ne_utimens_arg;

struct ne_utimens_res {
	int res;
};
typedef struct ne_utimens_res ne_utimens_res;

struct ne_open_arg {
	char *path;
	int flags;
};
typedef struct ne_open_arg ne_open_arg;

struct ne_open_res {
	int res;
};
typedef struct ne_open_res ne_open_res;

struct ne_read_arg {
	char *path;
	uint64_t size;
	uint64_t offset;
};
typedef struct ne_read_arg ne_read_arg;

struct ne_read_res {
	char *buf;
	int res;
};
typedef struct ne_read_res ne_read_res;

struct ne_write_arg {
	char *path;
	char *buf;
	uint64_t size;
	uint64_t offset;
};
typedef struct ne_write_arg ne_write_arg;

struct ne_write_res {
	int res;
};
typedef struct ne_write_res ne_write_res;

struct ne_statfs_arg {
	int res;
};
typedef struct ne_statfs_arg ne_statfs_arg;

struct ne_statfs_res {
	int res;
};
typedef struct ne_statfs_res ne_statfs_res;

#define NEFSPROG 17770
#define NEFSVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define GETATTR 1
extern  enum clnt_stat getattr_1(ne_getattr_arg , ne_getattr_res *, CLIENT *);
extern  bool_t getattr_1_svc(ne_getattr_arg , ne_getattr_res *, struct svc_req *);
#define ACCESS 2
extern  enum clnt_stat access_1(ne_access_arg , ne_access_res *, CLIENT *);
extern  bool_t access_1_svc(ne_access_arg , ne_access_res *, struct svc_req *);
#define READLINK 3
extern  enum clnt_stat readlink_1(ne_readlink_arg , ne_readlink_res *, CLIENT *);
extern  bool_t readlink_1_svc(ne_readlink_arg , ne_readlink_res *, struct svc_req *);
#define READDIR 4
extern  enum clnt_stat readdir_1(ne_readdir_arg , ne_readdir_res *, CLIENT *);
extern  bool_t readdir_1_svc(ne_readdir_arg , ne_readdir_res *, struct svc_req *);
#define MKNOD 5
extern  enum clnt_stat mknod_1(ne_mknod_arg , ne_mknod_res *, CLIENT *);
extern  bool_t mknod_1_svc(ne_mknod_arg , ne_mknod_res *, struct svc_req *);
#define MKDIR 6
extern  enum clnt_stat mkdir_1(ne_mkdir_arg , ne_mkdir_res *, CLIENT *);
extern  bool_t mkdir_1_svc(ne_mkdir_arg , ne_mkdir_res *, struct svc_req *);
#define SYMLINK 7
extern  enum clnt_stat symlink_1(ne_symlink_arg , ne_symlink_res *, CLIENT *);
extern  bool_t symlink_1_svc(ne_symlink_arg , ne_symlink_res *, struct svc_req *);
#define UNLINK 8
extern  enum clnt_stat unlink_1(ne_unlink_arg , ne_unlink_res *, CLIENT *);
extern  bool_t unlink_1_svc(ne_unlink_arg , ne_unlink_res *, struct svc_req *);
#define RMDIR 9
extern  enum clnt_stat rmdir_1(ne_rmdir_arg , ne_rmdir_res *, CLIENT *);
extern  bool_t rmdir_1_svc(ne_rmdir_arg , ne_rmdir_res *, struct svc_req *);
#define RENAME 10
extern  enum clnt_stat rename_1(ne_rename_arg , ne_rename_res *, CLIENT *);
extern  bool_t rename_1_svc(ne_rename_arg , ne_rename_res *, struct svc_req *);
#define LINK 11
extern  enum clnt_stat link_1(ne_link_arg , ne_link_res *, CLIENT *);
extern  bool_t link_1_svc(ne_link_arg , ne_link_res *, struct svc_req *);
#define CHMOD 12
extern  enum clnt_stat chmod_1(ne_chmod_arg , ne_chmod_res *, CLIENT *);
extern  bool_t chmod_1_svc(ne_chmod_arg , ne_chmod_res *, struct svc_req *);
#define CHOWN 13
extern  enum clnt_stat chown_1(ne_chown_arg , ne_chown_res *, CLIENT *);
extern  bool_t chown_1_svc(ne_chown_arg , ne_chown_res *, struct svc_req *);
#define TRUNCATE 14
extern  enum clnt_stat truncate_1(ne_truncate_arg , ne_truncate_res *, CLIENT *);
extern  bool_t truncate_1_svc(ne_truncate_arg , ne_truncate_res *, struct svc_req *);
#define UTIMENS 15
extern  enum clnt_stat utimens_1(ne_utimens_arg , ne_utimens_res *, CLIENT *);
extern  bool_t utimens_1_svc(ne_utimens_arg , ne_utimens_res *, struct svc_req *);
#define OPEN 16
extern  enum clnt_stat open_1(ne_open_arg , ne_open_res *, CLIENT *);
extern  bool_t open_1_svc(ne_open_arg , ne_open_res *, struct svc_req *);
#define READ 17
extern  enum clnt_stat read_1(ne_read_arg , ne_read_res *, CLIENT *);
extern  bool_t read_1_svc(ne_read_arg , ne_read_res *, struct svc_req *);
#define WRITE 18
extern  enum clnt_stat write_1(ne_write_arg , ne_write_res *, CLIENT *);
extern  bool_t write_1_svc(ne_write_arg , ne_write_res *, struct svc_req *);
#define STATFS 19
extern  enum clnt_stat statfs_1(ne_statfs_arg , ne_statfs_res *, CLIENT *);
extern  bool_t statfs_1_svc(ne_statfs_arg , ne_statfs_res *, struct svc_req *);
extern int nefsprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define GETATTR 1
extern  enum clnt_stat getattr_1();
extern  bool_t getattr_1_svc();
#define ACCESS 2
extern  enum clnt_stat access_1();
extern  bool_t access_1_svc();
#define READLINK 3
extern  enum clnt_stat readlink_1();
extern  bool_t readlink_1_svc();
#define READDIR 4
extern  enum clnt_stat readdir_1();
extern  bool_t readdir_1_svc();
#define MKNOD 5
extern  enum clnt_stat mknod_1();
extern  bool_t mknod_1_svc();
#define MKDIR 6
extern  enum clnt_stat mkdir_1();
extern  bool_t mkdir_1_svc();
#define SYMLINK 7
extern  enum clnt_stat symlink_1();
extern  bool_t symlink_1_svc();
#define UNLINK 8
extern  enum clnt_stat unlink_1();
extern  bool_t unlink_1_svc();
#define RMDIR 9
extern  enum clnt_stat rmdir_1();
extern  bool_t rmdir_1_svc();
#define RENAME 10
extern  enum clnt_stat rename_1();
extern  bool_t rename_1_svc();
#define LINK 11
extern  enum clnt_stat link_1();
extern  bool_t link_1_svc();
#define CHMOD 12
extern  enum clnt_stat chmod_1();
extern  bool_t chmod_1_svc();
#define CHOWN 13
extern  enum clnt_stat chown_1();
extern  bool_t chown_1_svc();
#define TRUNCATE 14
extern  enum clnt_stat truncate_1();
extern  bool_t truncate_1_svc();
#define UTIMENS 15
extern  enum clnt_stat utimens_1();
extern  bool_t utimens_1_svc();
#define OPEN 16
extern  enum clnt_stat open_1();
extern  bool_t open_1_svc();
#define READ 17
extern  enum clnt_stat read_1();
extern  bool_t read_1_svc();
#define WRITE 18
extern  enum clnt_stat write_1();
extern  bool_t write_1_svc();
#define STATFS 19
extern  enum clnt_stat statfs_1();
extern  bool_t statfs_1_svc();
extern int nefsprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_nefs_stat (XDR *, nefs_stat*);
extern  bool_t xdr_ne_dirent (XDR *, ne_dirent*);
extern  bool_t xdr_ne_getattr_arg (XDR *, ne_getattr_arg*);
extern  bool_t xdr_ne_getattr_res (XDR *, ne_getattr_res*);
extern  bool_t xdr_ne_access_arg (XDR *, ne_access_arg*);
extern  bool_t xdr_ne_access_res (XDR *, ne_access_res*);
extern  bool_t xdr_ne_readlink_arg (XDR *, ne_readlink_arg*);
extern  bool_t xdr_ne_readlink_res (XDR *, ne_readlink_res*);
extern  bool_t xdr_ne_readdir_arg (XDR *, ne_readdir_arg*);
extern  bool_t xdr_ne_readdir_res (XDR *, ne_readdir_res*);
extern  bool_t xdr_ne_mknod_arg (XDR *, ne_mknod_arg*);
extern  bool_t xdr_ne_mknod_res (XDR *, ne_mknod_res*);
extern  bool_t xdr_ne_mkdir_arg (XDR *, ne_mkdir_arg*);
extern  bool_t xdr_ne_mkdir_res (XDR *, ne_mkdir_res*);
extern  bool_t xdr_ne_unlink_arg (XDR *, ne_unlink_arg*);
extern  bool_t xdr_ne_unlink_res (XDR *, ne_unlink_res*);
extern  bool_t xdr_ne_rmdir_arg (XDR *, ne_rmdir_arg*);
extern  bool_t xdr_ne_rmdir_res (XDR *, ne_rmdir_res*);
extern  bool_t xdr_ne_symlink_arg (XDR *, ne_symlink_arg*);
extern  bool_t xdr_ne_symlink_res (XDR *, ne_symlink_res*);
extern  bool_t xdr_ne_rename_arg (XDR *, ne_rename_arg*);
extern  bool_t xdr_ne_rename_res (XDR *, ne_rename_res*);
extern  bool_t xdr_ne_link_arg (XDR *, ne_link_arg*);
extern  bool_t xdr_ne_link_res (XDR *, ne_link_res*);
extern  bool_t xdr_ne_chmod_arg (XDR *, ne_chmod_arg*);
extern  bool_t xdr_ne_chmod_res (XDR *, ne_chmod_res*);
extern  bool_t xdr_ne_chown_arg (XDR *, ne_chown_arg*);
extern  bool_t xdr_ne_chown_res (XDR *, ne_chown_res*);
extern  bool_t xdr_ne_truncate_arg (XDR *, ne_truncate_arg*);
extern  bool_t xdr_ne_truncate_res (XDR *, ne_truncate_res*);
extern  bool_t xdr_ne_utimens_arg (XDR *, ne_utimens_arg*);
extern  bool_t xdr_ne_utimens_res (XDR *, ne_utimens_res*);
extern  bool_t xdr_ne_open_arg (XDR *, ne_open_arg*);
extern  bool_t xdr_ne_open_res (XDR *, ne_open_res*);
extern  bool_t xdr_ne_read_arg (XDR *, ne_read_arg*);
extern  bool_t xdr_ne_read_res (XDR *, ne_read_res*);
extern  bool_t xdr_ne_write_arg (XDR *, ne_write_arg*);
extern  bool_t xdr_ne_write_res (XDR *, ne_write_res*);
extern  bool_t xdr_ne_statfs_arg (XDR *, ne_statfs_arg*);
extern  bool_t xdr_ne_statfs_res (XDR *, ne_statfs_res*);

#else /* K&R C */
extern bool_t xdr_nefs_stat ();
extern bool_t xdr_ne_dirent ();
extern bool_t xdr_ne_getattr_arg ();
extern bool_t xdr_ne_getattr_res ();
extern bool_t xdr_ne_access_arg ();
extern bool_t xdr_ne_access_res ();
extern bool_t xdr_ne_readlink_arg ();
extern bool_t xdr_ne_readlink_res ();
extern bool_t xdr_ne_readdir_arg ();
extern bool_t xdr_ne_readdir_res ();
extern bool_t xdr_ne_mknod_arg ();
extern bool_t xdr_ne_mknod_res ();
extern bool_t xdr_ne_mkdir_arg ();
extern bool_t xdr_ne_mkdir_res ();
extern bool_t xdr_ne_unlink_arg ();
extern bool_t xdr_ne_unlink_res ();
extern bool_t xdr_ne_rmdir_arg ();
extern bool_t xdr_ne_rmdir_res ();
extern bool_t xdr_ne_symlink_arg ();
extern bool_t xdr_ne_symlink_res ();
extern bool_t xdr_ne_rename_arg ();
extern bool_t xdr_ne_rename_res ();
extern bool_t xdr_ne_link_arg ();
extern bool_t xdr_ne_link_res ();
extern bool_t xdr_ne_chmod_arg ();
extern bool_t xdr_ne_chmod_res ();
extern bool_t xdr_ne_chown_arg ();
extern bool_t xdr_ne_chown_res ();
extern bool_t xdr_ne_truncate_arg ();
extern bool_t xdr_ne_truncate_res ();
extern bool_t xdr_ne_utimens_arg ();
extern bool_t xdr_ne_utimens_res ();
extern bool_t xdr_ne_open_arg ();
extern bool_t xdr_ne_open_res ();
extern bool_t xdr_ne_read_arg ();
extern bool_t xdr_ne_read_res ();
extern bool_t xdr_ne_write_arg ();
extern bool_t xdr_ne_write_res ();
extern bool_t xdr_ne_statfs_arg ();
extern bool_t xdr_ne_statfs_res ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_NEFS_H_RPCGEN */
