/* based on crc64_example.c */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "crc64.h"

#define BUF_SIZE 8192
#define INIT_SEED 0x12345678


int main(int argc, char *argv[])
{
	// uint8_t inbuf[BUF_SIZE];
	uint64_t avail_in, total_in = 0;
	uint64_t crc64_checksum;
	// FILE *in;

	if (argc != 2 && argc != 3) {
		fprintf(stderr, "Usage: checksum infile [length]\n    If length is > size of file, it will be set to size of file\n");
		exit(1);
	}

    int f = open(argv[1], O_RDONLY);
	if(f == -1) {
		fprintf(stderr, "Can't open %s for reading\n", argv[1]);
        exit(2);
	}
	size_t length = -1;
	if (argc == 3) {
		sscanf(argv[2], "%zu", &length);
	}
	
	struct stat stat;
	int ret = fstat(f, &stat);
	if(ret) {
		fprintf(stderr, "Can't stat %s\n", argv[1]);
		exit(2);
	}
	if (length > stat.st_size) {
		length = stat.st_size;
	}
    

    char *mem = mmap(NULL, length, PROT_READ, MAP_SHARED, f, 0);
    if(mem == MAP_FAILED) {
		fprintf(stderr, "ERR: memory mapping failed\n");
		exit(2);
	}
	close(f);
	crc64_checksum = INIT_SEED;
    crc64_checksum = crc64_ecma_refl(crc64_checksum, mem, length);



	// in = fopen(argv[1], "rb");
	// if (!in) {
	// 	exit(0);
	// }

	// // printf("crc64_example -- crc64_ecma_refl:\n");
	// fflush(0);

	// crc64_checksum = INIT_SEED;
	// while ((avail_in = fread(inbuf, 1, BUF_SIZE, in))) {
	// 	// crc update mode
	// 	crc64_checksum = crc64_ecma_refl(crc64_checksum, inbuf, avail_in);
	// 	total_in += avail_in;
	// }

	// fclose(in);
	printf("%lx\n", crc64_checksum);

	return 0;
}
