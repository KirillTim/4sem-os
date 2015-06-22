#include <bufio.h>

#define BUF_SIZE 4096

int main() {
    buf_t *buf = buf_new(BUF_SIZE);
    for (;;) {
        ssize_t res = buf_fill(STDIN_FILENO, buf, buf_capacity(buf)); 
        if (res < 0) {
            print_error();
            if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) < 0) {
                return 2;
            }
            return 1;
        }
        if (res == 0) {
            break;
        }
        if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) < 0) {
            return 2;
        }
    }
    return 0;
}
