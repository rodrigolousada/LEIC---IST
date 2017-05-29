# Makefile de exemplo

all: par-shell fibonacci

par-shell: commandlinereader.o par-shell.o
	gcc -o par-shell commandlinereader.o par-shell.o

fibonacci: fibonacci.c
	gcc -Wall -g -o fibonacci fibonacci.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

par-shell.o: par-shell.c commandlinereader.h
	gcc -Wall -g -c par-shell.c

clean:
	rm -f *.o par-shell fibonacci
