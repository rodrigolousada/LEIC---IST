#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include "list.h"
#include "commandlinereader.h"

#define VECTOR_SIZE 7
#define MAXPAR 2

int status, maxparvar = MAXPAR, iteration = -1, counter = 0, exiter = 0, total_time = 0;
list_t* list;
FILE *myfile;
pthread_mutex_t Locker;
pthread_cond_t Semaphore, Sleep;

void *MonitorThread(void *useless) {
	pid_t pid;
	
	while(1) {
		pthread_mutex_lock(&Locker);
		while (counter==0 && exiter==0) pthread_cond_wait(&Sleep, &Locker);
		
		if(exiter==1 && counter==0) /* For exit */ {
			pthread_mutex_unlock(&Locker);
			break;
		}
		
		pthread_mutex_unlock(&Locker);
		pid = wait(&status);
		pthread_mutex_lock(&Locker);
		total_time = update_terminated_process(list, pid, status, time(NULL), iteration, total_time, myfile);
		iteration++;
		counter--;
		pthread_cond_signal(&Semaphore);		
		pthread_mutex_unlock(&Locker);
	}
	pthread_exit(NULL);
}


int main() {
	pid_t pid;
	int n, status2;
	char *argVector[VECTOR_SIZE];
	pthread_t monitorthread;
	
	
	char line[100];
	myfile = fopen("log.txt", "a+");
	
	if(myfile!=NULL) {
		while(fgets(line, sizeof(line), myfile)!=NULL) {
			sscanf(line, "iteracao %d", &iteration);
			fgets(line, sizeof(line), myfile);
			fgets(line, sizeof(line), myfile);
			sscanf(line, "total execution time: %d s", &total_time);
		}
		iteration++;
	}

	list = lst_new();
	
	if (pthread_cond_init(&Semaphore, NULL) || pthread_cond_init(&Sleep, NULL)) {
		perror("Erro a inicializar as variaveis de condicao");
		exit(EXIT_FAILURE);
	}
	
	pthread_mutex_init(&Locker, NULL);
	pthread_create(&monitorthread, NULL, MonitorThread, NULL);

	while(1) {
		n = readLineArguments(argVector, VECTOR_SIZE);
		if(n == 0) /* In case of dont give any argument */
		  continue;
		 
		if(strcmp(argVector[0],"exit")==0) /* For exit */ {
			pthread_mutex_lock(&Locker);
			exiter = 1;
			pthread_cond_signal(&Sleep);
			pthread_mutex_unlock(&Locker);
			break;
		}
		
		if(strcmp(argVector[0],"more")==0) /* For increment maxparvar */ {
			pthread_mutex_lock(&Locker);
			maxparvar++;
			pthread_mutex_unlock(&Locker);
			continue;
		}
		
		pthread_mutex_lock(&Locker);
		while(counter == maxparvar) pthread_cond_wait(&Semaphore, &Locker);
		pthread_mutex_unlock(&Locker);
		pid = fork();
		if (pid == -1) {
			perror("fork error");
			exit(EXIT_FAILURE);
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
			pthread_cond_signal(&Sleep);
			pthread_mutex_unlock(&Locker);
		}
	}
	
	pthread_join(monitorthread, NULL);
	pthread_mutex_destroy(&Locker);
	pthread_cond_destroy(&Semaphore);
	pthread_cond_destroy(&Sleep);
	lst_destroy(list);
	fclose(myfile);
	return(0);
}
