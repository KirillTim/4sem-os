all: libhelpers libbufio

libhelpers: helpers.o
	gcc -shared helpers.o -o libhelpers.so
helpers.o:
	gcc -c -fPIC -std=c99 helpers.c -o helpers.o
helpclean:
	rm -f helpers.o
	rm -f libhelpers.so

libbufio: bufio.o
	gcc -shared bufio.o -o libbufio.so -L lhelpers
bufio.o:
	gcc -c -fPIC -std=c99 bufio.c -o bufio.o 
bufclean:
	rm -f bufio.o
	rm -f libbufio.so

clean: helpclean bufclean
