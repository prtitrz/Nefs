CC = gcc
CFLAGS = -Wall -lnsl `pkg-config fuse --cflags --libs`

all: rpc slave master nefs

rpc:
	rpcgen -MN nefs_cs.x
	rpcgen -MN nefs_cm.x

nefs: client.c debug.c nefs_cm_clnt.c nefs_cs_clnt.c nefs.c \
	nefs_cm_xdr.c nefs_cs_xdr.c
	$(CC) -o $@ $^ $(CFLAGS)

master: master.c debug.c nefs_cm_svc.c nefs_cm_xdr.c
	$(CC) -o $@ $^ $(CFLAGS)

slave: slave.c debug.c nefs_cs_svc.c nefs_cs_xdr.c
	$(CC) -o $@ $^ $(CFLAGS)
