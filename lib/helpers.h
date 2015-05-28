#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

ssize_t read_(int fd, void *buf, size_t count);
ssize_t write_(int fd, const void *buf, size_t count);
ssize_t read_until(int fd, void * buf, size_t count, char delimiter);
int spawn(const char * file, char * const argv []);
void print_error();

struct execargs_t {
    char* program;
    char** args; 
};
typedef struct execargs_t execargs_t;
execargs_t* execargs_new(char* program, char** args); 
int exec(execargs_t* args);
int runpiped(execargs_t** programs, size_t n);
#endif // HELPERS_H
