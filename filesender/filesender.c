#define _POSIX_C_SOURCE 201505
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <bufio.h>

#define BUF_SIZE 4096
#define QUEUE_SIZE 20

int main(int argc, char* argv[])
{   
    if (argc != 3) {
        fprintf(stderr, "Usage: ./filesender <port> <file name>\n");
        return 2;
    }
    struct addrinfo *result, *rp;
    if(getaddrinfo("localhost", argv[1], NULL, &result)) {
        return 1;
    }
    int sfd = socket(result->ai_family, SOCK_STREAM, 0);
    if (sfd < 0) {
        fprintf(stderr, "Can't create socket\n");
        return 1;
    }
    
    if (bind(sfd, result->ai_addr, result->ai_addrlen)) {
        fprintf(stderr, "Can't bind\n");
        return 1;
    }
    if (listen(sfd, QUEUE_SIZE)) {
        fprintf(stderr, "Can't listen()\n");
        return 1;
    }
    for (;;) {
        int ac = accept(sfd, NULL, NULL);
        if (ac < 0) {
            fprintf(stderr, "Can't accept()\n");
            return 1;
        }
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Can't fork()\n");
            return 3;
        } 
        else if (pid > 0) {
            close(ac);
            continue;
        }
        else {
            close(sfd);  
            fd_t file = open(argv[2], O_RDONLY);
            if (file < 0) {
                fprintf(stderr, "Can't read file\n");
                return 5;
            }
            buf_t* buf = buf_new(BUF_SIZE);
            if (!buf) {
                fprintf(stderr, "Can't create buf\n");
                return 5;
            }
            for (;;) {
                int res = buf_fill(file, buf, 1);
                if (res == 0) {
                    return 0;
                }
                if (res < 0) {
                    if (buf_flush(ac, buf, buf_size(buf)) < 0) {
                        return 6;
                    }
                    return 7;
                }
                if (buf_flush(ac, buf, buf_size(buf)) < 0) {
                    return 6;
                }
            }
        }
    }
    return 0;
}
