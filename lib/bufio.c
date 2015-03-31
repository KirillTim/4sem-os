#include "bufio.h"

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
    }
    rv->buf = (char*)malloc(capacity);
    if (rv->buf == NULL) {
        return NULL;
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
    ssize_t res = write_(fd, buf->buf, buf->size);  
    if (res == -1) {
        return res;
    }
    size_t now_size = buf->size - res;
    memmove(buf, buf->buf+res, now_size);
    buf->size = now_size;
    return now_size;
}

