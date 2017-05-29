#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commandlinereader.h"

#define VECTOR_SIZE 7

int main() {
	pid_t pid;
	int i, n, status, status2, counter=0;
	char *argVector[VECTOR_SIZE];

	while(1) {
		n = readLineArguments(argVector, VECTOR_SIZE);
		if(n == 0) /* In case of dont give any argument */
		  continue;
		  
		if(strcmp(argVector[0],"exit")==0) /* For exit */
		  break;
		
		pid = fork();
		if (pid == -1) {
			perror("fork error");
		}
		else if (pid == 0) {
			/* Child process */
			status2 = execv(argVector[0], argVector);
			perror("child problem");
			exit(status2);
		}
		else {
			/* Parent process */
			counter++;
		}
	}
	
	int status_array[counter];
	pid_t pid_array[counter];
	
	for (i=0; i<counter; i++) {
		pid_array[i] = wait(&status);
		status_array[i] = status;
	}
	
	for (i=0; i<counter; i++) {
		if(WIFEXITED(status_array[i])){
			printf("Process: %d(PID) ended with status %d\n", pid_array[i], WEXITSTATUS(status_array[i]));
		}
		else{
			printf("Process: %d(PID) ended without calling exit\n", pid_array[i]);
		}
	}
	
	return(0);
}
