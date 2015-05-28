#include "helpers.h"

ssize_t read_(int fd, void* buf, size_t count) {
	size_t offset = 0;
	for(;;) {
		ssize_t r = read(fd, buf + offset, count - offset);
		if(r == -1) {
			return -1;
		}
		offset += r;
		if(r == 0) {
			return offset;
		}
	}
}

ssize_t write_(int fd, const void* buf, size_t count) {
	size_t offset = 0;
	for(;;) {
		ssize_t r = write(fd, buf + offset, count - offset);
		if(r == -1) {
			return -1;
		}
		offset += r;
		if(offset >= count) {
			return offset;
		}
	}
}

ssize_t read_until(int fd, void * buf, size_t count, char delimiter) {
	ssize_t offset = 0;
	for(;;) {
		ssize_t r = read(fd, buf + offset, 1);
		if(r == -1) {
			return -1;
		}
		if(r == 0) {
		  return offset;
		}
		
		if(((char*) buf)[offset] == delimiter) {
			return offset+1;
		}
		if(offset == count) {
			return count;
		}
		offset ++;	
	}
}


int spawn(const char * file, char * const argv []) {
    int pid = fork();
    if (pid == 0) {
        int result = execvp(file, argv);
        exit(result);
    }
    else if (pid > 0) {
        int result;
        waitpid(pid, &result, 0);
        return result;
    }
    else {
        return -2;//impossible situation
    }
}

execargs_t* execargs_new(char* program, char** args) {
    execargs_t* rv = (execargs_t*)malloc(sizeof(execargs_t));
    return rv;
}

int exec(execargs_t* args) {
   return spawn(args->program, args->args); 
}

int runpiped(execargs_t** programs, size_t n) {
}

void print_error() {
	char* msg = strerror(errno);
	write_(STDERR_FILENO, msg, strlen(msg) * sizeof(char));
}
