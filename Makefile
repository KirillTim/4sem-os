all: libhelpers cat/cat revwords/revwords filter/filter bufcat/bufcat foreach/foreach filesender/filesender

libhelpers:
	cd lib && make
cat/cat: 
	cd cat && make
revwords/revwords:
	cd revwords && make
filter/filter:
	cd filter && make
bufcat/bufcat:
	cd bufcat && make
foreach/foreach:
	cd foreach && make
filesender/filesender:
	cd filesender && make
clean:
	cd lib && make clean
	cd cat && make clean
	cd revwords && make clean
	cd filter && make clean
	cd bufcat && make clean
	cd foreach && make clean
	cd filesender && make clean
