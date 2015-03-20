#include <stdio.h>
#include <helpers.h>

#define BUF_SIZE 4096+10

void reverse(char* str, size_t length) {
	char tmp;
	for (int i = 0; i < length/2; i++) {
		tmp = str[i];
		str[i] = str[length-1-i];
		str[length-1-i] = tmp;
	}
}

int main() {
	char buf[BUF_SIZE];
	ssize_t read_count;
	while ((read_count = read_until(STDIN_FILENO, buf, BUF_SIZE, ' ')) != 0) {
		if (read_count == -1) {
			print_error();
			return 1;
		}
		
		if (buf[read_count-1] == ' ') {
		  reverse(buf, read_count-1);
		}
		else 
		reverse(buf, read_count);
		ssize_t write_count = write_(STDOUT_FILENO, buf, read_count);	
	      	if (write_count == -1) {
			print_error();
			return 2;
		}
	}
	return 0;
}
