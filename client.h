#include <time.h>
#include "nefs_cs.h"
#include "nefs_cm.h"

extern int cm_getattr(ne_getattr_arg , ne_getattr_res *, char *);
extern int cm_access(ne_access_arg , ne_access_res *, char *);
extern int cm_readlink(ne_readlink_arg , ne_readlink_res *, char *);
extern int cm_readdir(ne_readdir_arg , ne_readdir_res *, char *);
extern int cm_mknod(ne_mknod_arg , ne_mknod_res *, char *);
extern int cm_mkdir(ne_mkdir_arg , ne_mkdir_res *, char *);
extern int cm_unlink(ne_unlink_arg , ne_unlink_res *, char *);
extern int cm_rmdir(ne_rmdir_arg , ne_rmdir_res *, char *);
extern int cm_symlink(ne_symlink_arg , ne_symlink_res *, char *);
extern int cm_rename(ne_rename_arg , ne_rename_res *, char *);
extern int cm_link(ne_link_arg , ne_link_res *, char *);
extern int cm_chmod(ne_chmod_arg , ne_chmod_res *, char *);
extern int cm_chown(ne_chown_arg , ne_chown_res *, char *);
extern int cs_truncate(ne_truncate_arg , ne_truncate_res *, char *);
//extern int cm_utimens(ne_utimens_arg , ne_utimens_res *, char *);
extern int cm_open(ne_open_arg , ne_open_res *, char *);

extern int cs_read(ne_read_arg , ne_read_res *, char *);
extern int cs_write(ne_write_arg , ne_write_res *, char *);
extern int cs_readsize(ne_readsize_arg , ne_readsize_res *, char *);

//extern int cm_statfs(ne_statfs_arg , ne_statfs_res *, char *);
