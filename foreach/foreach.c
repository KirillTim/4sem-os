#include <bufio.h>
#include <stdio.h>

int main() {
    buf_t* buf = buf_new(1000);
    char str[80];
    int res = buf_getline(STDIN_FILENO, buf, str);
    printf("res: %d\n", res);
    printf("str: %s\n", str);
        
    return 0;
}
