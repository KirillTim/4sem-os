#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    printf("check: %s\n", argv[1]);
    if (argv[1][0] == '1') {
        printf("ret 1\n");
        return 1;
    }
    printf("ret 0\n");
    return 0;
}
