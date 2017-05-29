# Makefile de exemplo

all: par-shell par-shell-terminal fibonacci

par-shell: commandlinereader.o par-shell.o list.o
	gcc -pthread -o par-shell commandlinereader.o list.o par-shell.o

par-shell-terminal: par-shell-terminal.o
	gcc -Wall -g -o par-shell-terminal commandlinereader.o par-shell-terminal.o

par-shell-terminal.o: par-shell-terminal.c commandlinereader.h
	gcc -Wall -g -c par-shell-terminal.c

fibonacci: fibonacci.c
	gcc -Wall -g -o fibonacci fibonacci.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

list.o: list.c list.h
	gcc -Wall -g -c list.c

par-shell.o: par-shell.c commandlinereader.h list.h
	gcc -Wall -g -c par-shell.c

clean:
	rm -f *.o par-shell fibonacci par-shell-terminal log.txt par-shell-out-*
