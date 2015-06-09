#include "bufio.h"

#include <stdio.h>

#ifdef DEBUG
#define ASSERT_DEBUG(condition) if(!(condition)) abort();
#else
#define ASSERT_DEBUG(condition)
#endif

buf_t* buf_new(size_t capacity) {
    buf_t* rv = (buf_t*)malloc(sizeof(buf_t));
    if (rv != NULL) {
        rv->capacity = capacity;
        rv->size = 0;
        rv->buf = (char*)malloc(capacity);
        if (rv->buf == NULL) {
            return NULL;
        }
    }
    return rv;
}

void buf_free(buf_t *buf) {
    free(buf->buf);
    free(buf);
}

size_t buf_capacity(buf_t *buf) {
    ASSERT_DEBUG(buf != NULL);
    return buf->capacity;
}

size_t buf_size(buf_t *buf) {
    ASSERT_DEBUG(buf != NULL);
    return buf->size;
}

int buf_is_full(buf_t *buf) {
    return buf->size == buf->capacity;
}

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required) {
    ASSERT_DEBUG(buf != NULL);
    ASSERT_DEBUG(required <= buf->capacity);
    while (buf->size < required) {
        ssize_t res = read(fd, buf->buf + buf->size, buf->capacity - buf->size);
        if (res == -1) {
            return res;
        }
        buf->size += res;
    }
    return buf->size;
}

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required) {
    ASSERT_DEBUG(buf != NULL);
    size_t count = 0;
    if (required > buf->size) {
        required = buf->size;
    }
    while (count < required) {
        ssize_t res = write(fd, buf->buf + count, buf->size-count);
        if (res == -1) {
            return res;
        }
        count += res;
    }
    size_t now_size = buf->size - count;
    memmove(buf->buf, buf->buf + count, now_size);
    buf->size = now_size;
    return count;
}

ssize_t buf_getline(fd_t fd, buf_t *buf, char *dest) {
    //printf("buf_getline!!!\n");
    int pl = 0;
    for (;;) {
        for (int i = pl; i < buf->size; i++) {
            if (buf->buf[i] == '\n') {
                memcpy(dest, buf->buf, i);
                memmove(buf->buf, buf->buf+i, buf->size-i);
                buf->size -= i;
                return i; 
            }
        }
        pl = buf->size;
        ssize_t res = buf_fill(fd, buf, buf->size + 1);
        //printf("res: %d", res);
        if (res == -1)
            return res;
    }
}

ssize_t buf_write(fd_t fd, buf_t *buf, char *src, size_t len) {
    while (len > 0) {
        size_t rem = buf->capacity - buf->size;
        size_t cp = rem < len ? rem : len;
        memcpy(buf->buf+buf->size, src, cp);
        len -= cp;
        buf->size += cp;
        ssize_t res = buf_flush(fd, buf, buf->size);
        if (res == -1)
            return res;
    }
}

