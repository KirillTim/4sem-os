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
		reverse(buf, read_count);
		ssize_t write_rs = write_(STDOUT_FILENO, buf, read_count);	
	}
	return 0;
}