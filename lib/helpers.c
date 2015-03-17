#include "helpers.h"

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
		ssize_t r = read(fd, buf + offset, 1);
		if(r == -1) {
			return -1;
		}
		if(r == 0) {
			return offset;
		}
		if(((char*) buf)[offset] == delimiter) {
			return offset;
		}
		if(offset == count) {
			return count;
		}
		offset ++;	
	}
}

void print_error() {
	char* msg = strerror(errno);
	write_(STDERR_FILENO, msg, strlen(msg) * sizeof(char));
}
