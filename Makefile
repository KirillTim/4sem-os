all: libhelpers cat/cat revwords/revwords filter/filter 

libhelpers:
	cd lib && make
cat/cat: 
	cd cat && make
revwords/revwords:
	cd revwords && make
filter/filter:
	cd filter && make
clean:
	cd lib && make clean
	cd cat && make clean
	cd revwords && make clean
	cd filter && make clean