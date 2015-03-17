#include "helpers.h"
#include <unistd.h>
#include <stdio.h>

ssize_t read_(int fd, void* buf, size_t count) {
	size_t offset = 0;
	for(;;) {
		ssize_t r = read(fd, buf + offset, count - offset);
		if(r == -1) {
			return -1;
		}
		offset += r;
		if(r == 0) {
			return offset;
		}
	}
}

ssize_t write_(int fd, const void* buf, size_t count) {
	size_t offset = 0;
	for(;;) {
		ssize_t r = write(fd, buf + offset, count - offset);
		if(r == -1) {
			return -1;
		}
		offset += r;
		if(offset >= count) {
			return offset;
		}
	}
}

ssize_t read_until(int fd, void * buf, size_t count, char delimiter) {
	ssize_t offset = 0;
	for(;;) {
		ssize_t r = read(fd, buf + offset, count - offset);
		fprintf( stderr, "offset: %zu \n" , offset);
		fprintf( stderr, "r: %zu \n" , r);
		for (int i = 0; i < r; i++) {
			if (((char*)buf)[i + offset] == delimiter) {
				fprintf( stderr, "i: %zu \n" , i);
				return offset + i;
			}
		}
		if (r == count) {
			return count;
		}
		if (r == -1) {
			return -1;
		}
		if (r == 0) {
			return offset;
		}

		offset += r;	
	}
}