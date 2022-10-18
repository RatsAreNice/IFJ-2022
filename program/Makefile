CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g
FILES= bottom_up.c Parser.c scanner.c symtable.c ASS.c ifj.c c206.c symdll.c semantic.c


all: compile 

testlolec: compile jihad

seg: compile valgrin

compile:
	$(CC) $(CFLAGS) $(FILES) -o ifj 

jihad:
	./ifj < testy/testassign.php > out.txt

valgrin: 
	valgrind ./ifj < testy/testassign.php > out.txt