#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "list.h"
#include "commandlinereader.h"

#define VECTOR_SIZE 7
#define MAX 100

int main(int argc, char** argv) {
	size_t useless;
	int fpipe, fstats;
	char* bufferStats= (char*) malloc(sizeof(char)*MAX), *line = (char*) malloc(sizeof(char)*MAX);
	char pathname[MAX] = "/tmp/", pidterminal[MAX] = "pidterminal ";
	
	if(argc!=2) {
		perror("More arguments than expected");
		exit(EXIT_FAILURE);
	}
	
	//Pipe Escrita
	strcat(pathname,argv[1]);
	if((fpipe = open(pathname, O_WRONLY)) < 0) {
		perror("Pipe open problem - terminal");
		exit(EXIT_FAILURE);
	}
	
	
	//Pipe Leitura Stats
	if((fstats = open("/tmp/stats", O_RDONLY)) < 0) {
		perror("Pipe open problem - terminal");
		exit(EXIT_FAILURE);
	}
	
	//Send pid to par-shell
	sprintf(pidterminal,"pidterminal %d\n", getpid());
	write(fpipe, pidterminal, strlen(pidterminal));
	
	while(1) {
		while(getline(&line, &useless, stdin) < 2);
		
		if(strcmp(line, "exit\n")==0) break;
        
		if(strcmp(line, "stats\n")==0) {
			write(fpipe, line, strlen(line));
			read(fstats, bufferStats, MAX);
			printf("%s", bufferStats);
			continue;
		}
		
		if(strcmp(line, "exit-global\n")==0) {
			write(fpipe, line, strlen(line));
			break;
		}
		
		write(fpipe, line, strlen(line));
		
	}
	
	close(fstats);
	close(fpipe);
	return 0;
}
