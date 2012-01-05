CC = gcc
CFLAGS = -Wall -lzmq `pkg-config fuse --cflags --libs`

#all: fec easyfec slave master nefs
all: local mds osd

local: nefs.c easyzmq.c
	$(CC) -o $@ $^ $(CFLAGS)

mds: mds.c easyzmq.c
	$(CC) -o $@ $^ $(CFLAGS)

osd: osd.c easyzmq.c
	$(CC) -o $@ $^ $(CFLAGS)

#slave: slave.c nefs_cs_svc.c nefs_cs_xdr.c
#	$(CC) -o $@ $^ $(CFLAGS)

#fec: fec.c
#	$(CC) -c $^ -std=c99

#easyfec: easyfec.c
#	$(CC) -c $^ -std=c99
