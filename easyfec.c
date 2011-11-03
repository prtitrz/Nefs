#include "fec.h"
#include "easyfec.h"

/*
 * The smallest integer k such that k*d >= n.
 */
int div_ceil(int n, int d)
{
	return ((n / d) + ((n % d) != 0));
}

int cal_miss(unsigned *index, int k)
{
	int i, miss;
	for (i = 0; i < k; i++) {
		if (index[i] != i) {
			miss++;
		}
	}
	return miss;
}

char * encode(char *data, int k, int m, int chunksize)
{
	fec_t *test;
	char *output;
	unsigned char *src[k];
	unsigned char *fecs[m-k];
	unsigned block[m-k];
	int i;

	output = (char *)malloc((m * chunksize + 1) * sizeof(char));
	for (i = 0; i < k; i++) {
		src[i] = output + i * chunksize;
		strncpy(src[i], data + (i * chunksize), chunksize);
	}
	for (i = 0; i < m - k; i++){
		block[i] = k + i;
		fecs[i] = output + (k + i) * chunksize;
	}
	test = fec_new(k, m);
	fec_encode(test, src, fecs, block, (m - k), chunksize);
	output[m * chunksize] = '\0';
	fec_free(test);
	return output;
}

char * decode(char *data, int k, int m, unsigned *index, int miss, int chunksize)
{
	fec_t *test;
	char *output;
	unsigned char *inpkts[k];
	unsigned char *outpkts[miss];
	int i, j=0, h;

	output = (char *)malloc((k * chunksize + 1) * sizeof(char));
	for (i = 0; i < k; i++) {
		inpkts[i] = data + index[i] * chunksize;
	}
	for (i = 0; i < k; i++) {
		if (index[i] != i) {
			outpkts[j] = output + i * chunksize; 
			j++;
		}
		else {
			for (h = 0; h < chunksize; h++) {
				output[i * chunksize + h] = inpkts[i][h];
			}
		}
	}
	test = fec_new(k, m);
	fec_decode(test, inpkts, outpkts, index, chunksize);
	output[k * chunksize] = '\0';
	fec_free(test);
	return output;
}

/*
int main(int argc, const char *argv[])
{
	unsigned index[4] = {0,4,2,7};
	char data[] = "testaaaabbbbccccfjdka";
	char *buf, *buf2;
	int chunksize, miss;

	chunksize = div_ceil(strlen(data), 4);
	buf = encode(data, 4, 8, chunksize);
	miss = cal_miss(index, 4);
	buf2 = decode(buf, 4, 8, index, miss, chunksize);
	printf("%s\n", buf2);
	return 0;
}
*/
