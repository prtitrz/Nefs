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

program CSPROG {
	version CSVERS {
		ne_read_res READ(ne_read_arg) = 1;
		ne_write_res WRITE(ne_write_arg) = 2;
	} = 1;
} = 17777;
