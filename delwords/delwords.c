#include <helpers.h>
#include <stdio.h>
#include <string.h>

#define WORD_SIZE 1024
#define BUF_SIZE 10

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: delwords <word>\n");
		return 1;
	}
	char buf[BUF_SIZE];
	ssize_t read_count;
	size_t word_len = strlen(argv[1]);
	int start = 0;
	int len = 0;
	while (1) {
		read_count = read_(STDIN_FILENO, buf+len, BUF_SIZE-len);
		len += read_count;
		if (read_count == 0) {
			write_(STDOUT_FILENO, buf, len);
			return 0;
		}
		if (read_count == -1) {
			print_error();
			return 1;
		}

			
		for (int i = 0; i < len; i++, start ++ ) {
			if (len - i < word_len) {
				memmove(buf, buf + i, len - i);
				len = len - i;
				start = 0;
				break;	
			}
			int j = 0;
			for (; j < word_len; j++) {
				if (buf[i+j] != argv[1][j]) {
					break;
				}
			}
			if (j == word_len) {
				i += word_len - 1;
				start = i;				
			}
			else {
				write_(STDOUT_FILENO, buf+i, 1);
			}
			}
		if (start == len) {
			start = len = 0;
		}
	}
	return 0;
}