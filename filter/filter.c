#include <helpers.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


#define BUF_SIZE 4096

int main(int argc, char* argv[]) {
    char buf[BUF_SIZE];
    char* file = argv[1];
    for(int i = 0; i < argc - 1; i++)
        argv[i] = argv[i + 1];
    argv[argc - 1] = buf;

    for(;;) {
        int count = read_until(STDIN_FILENO, buf, BUF_SIZE - 1, '\n');
        if (count == -1) {
            print_error();
            return 1;
        } 
        else if (count == 0) {
            return 0;
        }
        char last_n = buf[count-1] == '\n';
        if (last_n) {
          buf[count-1] = 0;
        }
        int stdout_backup = dup(1);
        close(1);
        int fd = open("/dev/null", O_WRONLY);
        dup2(1, fd);
        
        int code = spawn(file, argv);
        
        dup2(stdout_backup, 1);
        if(code == 0) {            
            if (last_n) {
              buf[count-1] = '\n';
              count ++;
            }
            if(write_(STDOUT_FILENO, buf, count) < 0)
                return 2;
        }
    }
}
