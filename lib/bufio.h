#ifndef BUFIO_H
#define BUFIO_H

#include "helpers.h"
#include <stdlib.h>

struct buf_t {
    int capacity;
    int size;
    char* buf;
};
typedef struct buf_t buf_t;
typedef int fd_t; 
buf_t *buf_new(size_t capacity);
void buf_free(buf_t *);
size_t buf_capacity(buf_t *);
size_t buf_size(buf_t *);
int buf_is_full(buf_t *);
int buf_is_empty(buf_t *);
ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required);
ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required);
ssize_t buf_getline(int fd, buf_t *buf, char sep, void *ebuf);
ssize_t buf_write(fd_t fd, buf_t *buf, char *src, size_t len);
#endif // BUFIO_H
