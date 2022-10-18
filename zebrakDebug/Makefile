CC=gcc
CFLAGS=-Wall -std=c11 -pedantic
FILE=testASS.c ../program/ASS.c



test: compileASS runASS cleanASS

valgr: compileASS memtestASS cleanASS

ifjc22: compileASS interpret

compileASS:
	$(CC) $(CFLAGS) $(FILE) -o testASo

runASS:
	./testASo > out.txt

cleanASS:
	rm -f testASo

memtestASS:
	valgrind ./testASo

valgrindASS: compileASS memtestASS cleanASS

interpret:
	./testASo > out.txt
	./ic22int out.txt > result.txt
	