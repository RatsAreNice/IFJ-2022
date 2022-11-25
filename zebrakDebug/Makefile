CC=gcc
CFLAGS=-Wall -std=c11 -pedantic
FILE=testtable.c


testtable: compile run clean

#testASS: compileASS runASS cleanASS

valgrind: compile memtest clean

#valgrindASS: compileASS memtestASS cleanASS

#compileASS:
#	gcc -Wall -std=c11 -pedantic testASS.c ../Source/ASS.c -o testASSo

#runASS:
#	./testASSo

#cleanASS:
#	rm -f testASSo

#memtestASS:
#	valgrind ./testASSo

compile:
	$(CC) $(CFLAGS) $(FILE) -o testtable 

run:
	./testtable
#sem pridat nejaky text file streamovany do ./ifj
clean:
	rm -f testtable


memtest:
	valgrind ./testtable