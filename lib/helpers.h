#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

ssize_t read_(int fd, void *buf, size_t count);
ssize_t write_(int fd, const void *buf, size_t count);
ssize_t read_until(int fd, void * buf, size_t count, char delimiter);
void print_error();

#endif // HELPERS_H
