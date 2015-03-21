all: libhelpers cat/cat revwords/revwords delwords/delwords

libhelpers:
	cd lib && make
cat/cat: 
	cd cat && make
revwords/revwords:
	cd revwords && make
delwords/delwords:
	cd delwords && make
clean:
	cd lib && make clean
	cd cat && make clean
	cd revwords && make clean
	cd delwords && make clean