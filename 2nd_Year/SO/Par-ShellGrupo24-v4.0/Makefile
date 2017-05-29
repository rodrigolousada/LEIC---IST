# Makefile de exemplo

all: par-shell fibonacci

par-shell: commandlinereader.o par-shell.o list.o
	gcc -pthread -o par-shell commandlinereader.o list.o par-shell.o

fibonacci: fibonacci.c
	gcc -Wall -g -o fibonacci fibonacci.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

list.o: list.c list.h
	gcc -Wall -g -c list.c

par-shell.o: par-shell.c commandlinereader.h list.h
	gcc -Wall -g -c par-shell.c

clean:
	rm -f *.o par-shell fibonacci
