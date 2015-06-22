all: forking polling
forking:
	gcc -std=c99 -o forking forking.c -I../lib -L../lib -lbufio -lhelpers
polling:
	gcc -std=c99 -o polling polling.c -I../lib -L../lib -lbufio -lhelpers

clean:
	rm -f ./forking
	rm -f ./polling
