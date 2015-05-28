#include "bufio.h"

#include <stdio.h>

#ifdef DEBUG
#define ASSERT_DEBUG(condition) if(!(condition)) abort();
#else
#define ASSERT_DEBUG(condition)
#endif

buf_t *buf_new(size_t capacity) {
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

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required) {
    ASSERT_DEBUG(buf != NULL);
    ASSERT_DEBUG(required <= buf->capacity);
    ssize_t res = read_(fd, buf->buf + buf->size, required - buf->size);
    if (res == -1) {
       return res;
    }
    buf->size += res;
    return buf->size;
}

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required) {
    ASSERT_DEBUG(buf != NULL);
    ssize_t res = write_(fd, buf->buf, required);  
    if (res == -1) {
        return res;
    }
    size_t now_size = buf->size - res;
    memmove(buf->buf, buf->buf+res, now_size);
    buf->size = now_size;
    return now_size;
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
        memcpy(buf->buf+buf->capacity, src, cp);
        len -= cp;
        buf->size += cp;
        ssize_t res = buf_flush(fd, buf, buf->size);
        if (res == -1)
            return res;
    }
}

