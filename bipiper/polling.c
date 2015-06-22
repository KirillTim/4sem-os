#define _POSIX_C_SOURCE 201505
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <bufio.h>

#define BUF_SIZE 4096
#define MAX_CLIENTS_SIZE 256
#define QUEUE_SIZE 10

int closed[MAX_CLIENTS_SIZE][2] = {};
struct pollfd clients[MAX_CLIENTS_SIZE];
buf_t* buffers[127*2];
int buffers_size;
int clients_size;
int pairs_count;

int init_socket(char* port) {
    struct addrinfo *result, *rp;
    if (getaddrinfo("localhost", port, NULL, &result)) {
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
    fprintf(stderr, "sfd: %d\n", sfd);
    return sfd;
}

buf_t* get_read_from_buffer(int client_number) {
    return buffers[client_number];
}

buf_t* get_write_to_buffer(int client_number) {
    int buf_number;
    if (client_number % 2 == 0) {
        buf_number = client_number + 1; 
    }
    else {
        buf_number = client_number - 1;
    }
    return buffers[buf_number];
}

int read_from(int client_number) {
    buf_t* buf = get_read_from_buffer(client_number);
    int fd = clients[2 + client_number].fd;
    int res = buf_fill(fd,buf, 1);
    return res;
}

int write_to(int client_number) {
    buf_t* buf = get_write_to_buffer(client_number);
    int fd = clients[2 + client_number].fd;
    int res = buf_flush(fd, buf, 1);
    return res;
}

void add_clients(int* ac) {
    fprintf(stderr, "ac: %d, %d\n", ac[0], ac[1]);
    clients[clients_size].fd = ac[0];
    clients[clients_size].events = POLLIN | POLLOUT;
    clients[clients_size].revents = 0;
    clients[clients_size+1].fd = ac[1];
    clients[clients_size+1].events = POLLIN | POLLOUT;
    clients[clients_size+1].revents = 0;
    clients_size += 2;
    buffers[buffers_size] = buf_new(BUF_SIZE);
    buffers[buffers_size+1] = buf_new(BUF_SIZE);
    buffers_size += 2;
    pairs_count ++;
}

int main(int argc, char* argv[])
{   
    if (argc != 3) {
        fprintf(stderr, "Usage: ./polling <port1> <port2>\n");
        return 2;
    }
    memset(clients, 0, sizeof(struct pollfd));
    clients[0].fd = init_socket(argv[1]);
    clients[0].events = POLLIN;
    clients[1].fd = init_socket(argv[2]); 
    clients[1].events = POLLIN;
    buffers_size = 0;
    clients_size = 2;
    pairs_count = 0;
    int ac[2] = {-1, -1};
    for (;;) {
        int r = poll(clients, clients_size, -1);  
        if (r == -1) {
            perror("poll()\n");
        }
        else {
            for (int i = 0; i < 2; i++) {
                if ((clients[i].revents & POLLIN) && ac[i] == -1) {
                    ac[i] = accept(clients[i].fd, NULL, NULL);
                }
            }
            if (ac[0] != -1 && ac[1] != -1) {
                add_clients(ac);
                ac[0] = ac[1] = -1;
            }
            for (int i = 0; i < pairs_count * 2; i++) {
                //fprintf(stderr, "i=%d: clients[2+i].fd= %d\n", i, clients[2 + i].fd);
                if (clients[2 + i].revents & POLLIN) {
                    //fprintf(stderr,"can read from: %d\n", i);
                    clients[2 + i].revents = 0;
                    int r = read_from(i);
                    //fprintf(stderr, "read: %d\n", r);
                    if (r == -1) {
                        closed[i/2][0]++;                         
                    }
                }
                if (clients[2 + i].revents & POLLOUT) {
                    //fprintf(stderr, "can write to: %d\n", i);
                    clients[2 + i].revents = 0;
                    int r = write_to(i);
                    if (r == -1) {
                        closed[i/2][0]++;
                    }
                    //fprintf(stderr, "read: %d\n", r);
                }
            }
            for (int i = 0; i < pairs_count; i++) {
                if (closed[i][0] >= 4) {
                    
                }
            }
            for (int i = 0; i < pairs_count * 2; i++) {
                if ((clients[2 + i].events & POLLIN) 
                        && buf_is_full(get_read_from_buffer(i))) {
                    clients[2 + i].events = 0 | POLLOUT;
                }
                if ((clients[2+i].events & POLLOUT) 
                        && buf_is_empty(get_write_to_buffer(i))) {
                    clients[2 + i].events = 0 | POLLIN;
                }
                if (!buf_is_full(get_read_from_buffer(i))) {
                    clients[2 + i].events = 0 | POLLIN;
                }
                if (!buf_is_empty(get_write_to_buffer(i))) {
                    clients[2 + i].events = 0 | POLLOUT;
                }
            }
        }
    }

    return 0;
}
