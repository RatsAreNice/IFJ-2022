CC=gcc
CFLAGS=-Wall -std=c11 -pedantic
FILE=testASS.c ../Source/ASS.c



test: compileASS runASS cleanASS



compileASS:
	$(CC) $(CFLAGS) $(FILE) -o testASo

runASS:
	./testASo

cleanASS:
	rm -f testASo

memtestASS:
	valgrind ./testASo

valgrindASS: compileASS memtestASS cleanASS
