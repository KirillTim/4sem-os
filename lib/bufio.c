#include "bufio.h"

typedef struct buf_t buf_t;
typedef int fd_t;
buf_t *buf_new(size_t capacity) {
    buf_t* rv = (buf_t*)malloc(sizeof(buf_t));
    if (rv != NULL) {
        rv->capacity = capacity;
    }
    return rv;
}

void buf_free(buf_t *buf) {
    free(buf);
}

size_t buf_capacity(buf_t *buf) {
    return buf->capacity;
}

size_t buf_size(buf_t *buf) {
    return buf->size;
}

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required) {
   ssize_t res = read_(fd, buf->buf + buf->size, required - buf->size);
   if (res == -1) {
       return res;
    }
   buf->size += res;
   return buf->size;
}

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required) {
   ssize_t res = write_(fd, buf, buf->size);  
    if (res == -1) {
        return res;
    }
    size_t now_size = buf->size - res;
    memmove(buf, buf->buf+res, now_size);
    buf->size = now_size;
    return now_size;
}

