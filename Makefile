all: libhelpers cat revwords filter bufcat foreach

libhelpers:
	cd lib && make
cat: 
	cd cat && make
revwords:
	cd revwords && make
filter:
	cd filter && make
bufcat:
	cd bufcat && make
foreach:
	cd foreach && make
clean:
	cd lib && make clean
	cd cat && make clean
	cd revwords && make clean
	cd filter && make clean
	cd bufcat && make clean
	cd foreach && make clean
