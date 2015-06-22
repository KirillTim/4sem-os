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

void transfer(fd_t in, fd_t out) {
    fprintf(stderr, "transfer: in=%d, out=%d\n", in, out);
    buf_t* buf = buf_new(BUF_SIZE);
    if (!buf) {
        fprintf(stderr, "Can't create buf\n");
        exit(10);
    }
    for (;;) {
        int res = buf_fill(in, buf, 1);
        buf_flush(out, buf, buf_size(buf));
        if (res == 0) {
            shutdown(out, SHUT_WR);
            return ;
        }
        if (res < 0) {
            return ;
        }
    }
}

int init_socket(char* port) {
    struct addrinfo *result, *rp;
    if(getaddrinfo("localhost", port, NULL, &result)) {
        exit(1);
    }
    int sfd = socket(result->ai_family, SOCK_STREAM, 0);
    if (sfd < 0) {
        fprintf(stderr, "Can't create socket\n");
        exit(1);
    }
    
    if (bind(sfd, result->ai_addr, result->ai_addrlen)) {
        fprintf(stderr, "Can't bind\n");
        exit(1);
    }
    if (listen(sfd, QUEUE_SIZE)) {
        fprintf(stderr, "Can't listen()\n");
        exit(1);
    }
    return sfd;
}

int main(int argc, char* argv[])
{   
    if (argc != 3) {
        fprintf(stderr, "Usage: ./filesender <port1> <port2>\n");
        return 2;
    }
    int sfd1 = init_socket(argv[1]);
    int sfd2 = init_socket(argv[2]);
    for (;;) {
        int ac1 = accept(sfd1, NULL, NULL);
        if (ac1 < 0) {
            fprintf(stderr, "Can't accept first socket()\n");
            return 2;
        }
        int ac2 = accept(sfd2, NULL, NULL);
        if (ac2 < 0) {
            fprintf(stderr, "Can't accept second socket()\n");
            return 2;
        }
        fprintf(stderr, "Two connected!\n");
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Can't fork()\n");
            return 3;
        } 
        else if (pid == 0) {
            transfer(ac1, ac2);
            close(ac1);
            close(ac2);
            exit(0);
        }
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Can't fork()\n");
            return 3;
        }
        else if (pid == 0) {
            transfer(ac2, ac1);
            close(ac1);
            close(ac2);
            exit(0);
        }
        close(ac1);
        close(ac2);
    }
    return 0;
}
