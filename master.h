#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <limits.h>
#include <stdlib.h>
#include "nefs_cm.h"
#include "debug.h"

const char *MASTER_PATH = "/home/rz/master";
