all: libhelpers cat/cat revwords/revwords

libhelpers:
	cd lib && make
cat/cat: 
	cd cat && make
revwords/revwords:
	cd revwords && make
clean:
	cd lib && make clean
	cd cat && make clean
	cd revwords && make clean