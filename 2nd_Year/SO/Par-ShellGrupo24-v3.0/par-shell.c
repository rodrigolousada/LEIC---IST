#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>
#include "list.h"
#include "commandlinereader.h"

#define VECTOR_SIZE 7
#define MAXPAR 4

int status, counter = 0, exiter = 0;
list_t* list;
pthread_mutex_t Locker;
sem_t Semaphore;
sem_t Sleep;

void *MonitorThread(void *useless) {
	pid_t pid;
	
	while(1) {
		pthread_mutex_lock(&Locker);
		if (counter > 0) {
			pthread_mutex_unlock(&Locker);
			pid = wait(&status);
			pthread_mutex_lock(&Locker);
			update_terminated_process(list, pid, status, time(NULL));
			counter--;
			pthread_mutex_unlock(&Locker);
			sem_post(&Semaphore);
		}
		else{
			pthread_mutex_unlock(&Locker);
			sem_wait(&Sleep);
		}
		
		pthread_mutex_lock(&Locker);
		if(exiter==1 && counter==0) /* For exit */ {
			pthread_mutex_unlock(&Locker);
			break;
		}
		pthread_mutex_unlock(&Locker);
	}
	pthread_exit(NULL);
}


int main() {
	pid_t pid;
	int n, status2;
	char *argVector[VECTOR_SIZE];
	pthread_t monitorthread;

	list = lst_new();
	
	sem_init(&Semaphore, 0, MAXPAR);
	sem_init(&Sleep, 0, 0);
	pthread_mutex_init(&Locker, NULL);
	pthread_create(&monitorthread, NULL, MonitorThread, NULL);

	while(1) {
		n = readLineArguments(argVector, VECTOR_SIZE);
		if(n == 0) /* In case of dont give any argument */
		  continue;
		  
		if(strcmp(argVector[0],"exit")==0) /* For exit */ {
			exiter = 1;
			sem_post(&Sleep);
			break;
		}
		
		sem_wait(&Semaphore);
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
			pthread_mutex_lock(&Locker);
			insert_new_process(list, pid, time(NULL));
			counter++;
			pthread_mutex_unlock(&Locker);
			sem_post(&Sleep);
		}
	}
	
	pthread_join(monitorthread, NULL);
	lst_print(list);
	pthread_mutex_destroy(&Locker);
	sem_destroy(&Semaphore);
	sem_destroy(&Sleep);
	lst_destroy(list);
	return(0);
}
