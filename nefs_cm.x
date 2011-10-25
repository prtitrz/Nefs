/* according <sys/type.h> , <bits/types.h> and <bits/typesizes.h> */

/* according man 2 stat */
struct nefs_stat {
	uint64_t dev;
	uint64_t ino;
	unsigned int mode;
	uint32_t nlink;
	unsigned int uid;
	unsigned int gid;
	uint64_t rdev;
	uint64_t size;
	uint64_t atime;
	uint64_t mtime;
	uint64_t ctime;
	uint64_t blksize;
	uint64_t blocks;
};

/* man readdir .POSIX.1.*/
struct ne_dirent {
	uint64_t d_ino;
	string d_name<>;
	struct ne_dirent* next;
};

struct ne_getattr_arg {
	string path<>;
};

struct ne_getattr_res {
	struct nefs_stat stbuf;
	int res;
};

struct ne_access_arg {
	string path<>;
	int mask;
};

struct ne_access_res {
	int res;
};

struct ne_readlink_arg {
	string path<>;
	uint64_t size;
};

struct ne_readlink_res {
	string buf<>;
	int res;
};

struct ne_readdir_arg {
	string path<>;
};

struct ne_readdir_res {
	ne_dirent *dirent;
};

struct ne_mknod_arg {
	string path<>;
	unsigned int mode;
	uint64_t rdev;
};

struct ne_mknod_res {
	int res;
};

struct ne_mkdir_arg {
	string path<>;
	unsigned int mode;
};

struct ne_mkdir_res {
	int res;
};

struct ne_unlink_arg {
	string path<>;
};

struct ne_unlink_res {
	int res;
};

struct ne_rmdir_arg {
	string path<>;
};

struct ne_rmdir_res {
	int res;
};

struct ne_symlink_arg {
	string from<>;
	string to<>;
};

struct ne_symlink_res {
	int res;
};

struct ne_rename_arg {
	string from<>;
	string to<>;
};

struct ne_rename_res {
	int res;
};

struct ne_link_arg {
	string from<>;
	string to<>;
};

struct ne_link_res {
	int res;
};

struct ne_chmod_arg {
	string path<>;
	unsigned int mode;
};

struct ne_chmod_res {
	int res;
};

struct ne_chown_arg {
	string path<>;
	unsigned int uid;
	unsigned int gid;
};

struct ne_chown_res {
	int res;
};
/*
struct ne_truncate_arg {
	string path<>;
	uint64_t size;
};

struct ne_truncate_res {
	int res;
};
*/
struct ne_utimens_arg {
	int res;
};

struct ne_utimens_res {
	int res;
};

struct ne_open_arg {
	string path<>;
	int flags;
};

struct ne_open_res {
	int res;
};
/*
struct ne_read_arg {
	string path<>;
	uint64_t size;
	uint64_t offset;
};

struct ne_read_res {
	string buf<>;
	int res;
};

struct ne_write_arg {
	string path<>;
	string buf<>;
	uint64_t size;
	uint64_t offset;
};

struct ne_write_res {
	int res;
};
*/

struct ne_statfs_arg {
	int res;
};

struct ne_statfs_res {
	int res;
};

program NEFSPROG {
	version NEFSVERS {
		ne_getattr_res GETATTR(ne_getattr_arg) = 1;
		ne_access_res ACCESS(ne_access_arg) = 2;
		ne_readlink_res READLINK(ne_readlink_arg) = 3;
		ne_readdir_res READDIR(ne_readdir_arg) = 4;
		ne_mknod_res MKNOD(ne_mknod_arg) = 5;
		ne_mkdir_res MKDIR(ne_mkdir_arg) = 6;
		ne_symlink_res SYMLINK(ne_symlink_arg) = 7;
		ne_unlink_res UNLINK(ne_unlink_arg) = 8;
		ne_rmdir_res RMDIR(ne_rmdir_arg) = 9;
		ne_rename_res RENAME(ne_rename_arg) = 10;
		ne_link_res LINK(ne_link_arg) = 11;
		ne_chmod_res CHMOD(ne_chmod_arg) = 12;
		ne_chown_res CHOWN(ne_chown_arg) = 13;
/*		ne_truncate_res TRUNCATE(ne_truncate_arg) = 14;*/
		ne_utimens_res UTIMENS(ne_utimens_arg) = 15;
		ne_open_res OPEN(ne_open_arg) = 16;
/*		ne_read_res READ(ne_read_arg) = 17;
		ne_write_res WRITE(ne_write_arg) = 18;*/
		ne_statfs_res STATFS(ne_statfs_arg) = 19;
	} = 1;
} = 17770;

