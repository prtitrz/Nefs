#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int div_ceil(int n, int d);

int cal_miss(unsigned *index, int k);

char* encode(char *data, int k, int m, int chunksize);

char* decode(char *data, int k, int m, unsigned *index, int miss, int chunksize);
