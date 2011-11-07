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

struct ne_readsize_arg {
	string path<>;
};

struct ne_readsize_res {
	uint64_t size;
	int res;
};

struct ne_truncate_arg {
	string path<>;
	uint64_t size;
};

struct ne_truncate_res {
	int res;
};

program CSPROG {
	version CSVERS {
		ne_read_res READ(ne_read_arg) = 1;
		ne_write_res WRITE(ne_write_arg) = 2;
		ne_readsize_res READSIZE(ne_readsize_arg) = 3;
		ne_truncate_res TRUNCATE(ne_truncate_arg) = 4;
	} = 1;
} = 17777;
