#define _POSIX_C_SOURCE 1
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
#define MAXPAR 4
#define MAX 100

int status, iteration = -1, counter = 0, exiter = 0, exitSignaled = 0, total_time = 0, nterminals = 0, terminalpids[MAX];
list_t* list;
FILE *myfile;
pthread_mutex_t Locker;
pthread_cond_t Vcond, Sleep;

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
		pthread_cond_signal(&Vcond);		
		pthread_mutex_unlock(&Locker);
	}
	pthread_exit(NULL);
}

void killTerminal() {
	int i;
	for(i=0; i < nterminals; i++) {
		kill(terminalpids[i], SIGKILL);
	}
	nterminals = 0;
	exitSignaled = 1;
}

int main() {
	pid_t pid;
	int n, status2, fchild, fstats, fpipe;
	char *argVector[VECTOR_SIZE], title[MAX], line[MAX], bufferStats[MAX];
	pthread_t monitorthread;
	
	//Geting iteration and total_time values
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
	
	//Variaveis de condicao e Mutexs
	if (pthread_cond_init(&Vcond, NULL) || pthread_cond_init(&Sleep, NULL)) {
		perror("Erro a inicializar as variaveis de condicao");
		exit(EXIT_FAILURE);
	}
	
	if(pthread_mutex_init(&Locker, NULL)) {
		perror("Erro a inicializat o mutex");
		exit(EXIT_FAILURE);
	}
	
	//Pthread
	if(pthread_create(&monitorthread, NULL, MonitorThread, NULL)) {
		perror("Erro a criar a thread");
		exit(EXIT_FAILURE);
	}
	
	
	//Pipe de Leitura e Escrita
	unlink("/tmp/par-shell-in");
	if(mkfifo("/tmp/par-shell-in", 0777) < 0) {
		perror("Pipe mkfifo problem");
		exit(EXIT_FAILURE);
	}
	
	unlink("/tmp/stats");
	if(mkfifo("/tmp/stats", 0777) < 0) {
		perror("Pipe mkfifo problem - terminal");
		exit(EXIT_FAILURE);
	}
	
	if((fpipe = open("/tmp/par-shell-in", O_RDONLY)) < 0) {
		perror("Pipe open problem");
		exit(EXIT_FAILURE);
	}
	
	if((fstats = open("/tmp/stats", O_WRONLY)) < 0) {
		perror("Pipe open problem");
		exit(EXIT_FAILURE);
	}
	
	close(0);
	dup(fpipe);
	
	//Signal
	if((signal(SIGINT, killTerminal))==SIG_ERR) {
		perror("Signal problem");
		exit(EXIT_FAILURE);
	}

	while(1) {
		n = readLineArguments(argVector, VECTOR_SIZE);
		
		if(n <= 0 && !exitSignaled) /* In case of dont give any argument */
		// deveria fechar e voltar a abrir o fpipe para ele nao fritar
		  continue;
		 
		if(strcmp(argVector[0],"exit-global")==0 || exitSignaled) /* For exit */ {
			pthread_mutex_lock(&Locker);
			exiter = 1;
			pthread_cond_signal(&Sleep);
			pthread_mutex_unlock(&Locker);
			break;
		}
		
		if(strcmp(argVector[0], "pidterminal")==0) /* Regists a new terminal */{
			terminalpids[nterminals] = atoi(argVector[1]);
			nterminals++;
			continue;
		}
		
		if(strcmp(argVector[0], "stats")==0) /* Returns the stats to the terminal */{
			pthread_mutex_lock(&Locker);
			sprintf(bufferStats, "Child Processes Number: %d\nTotal Time: %d\n", counter, total_time);
			pthread_mutex_unlock(&Locker);
			write(fstats, bufferStats, strlen(bufferStats));
			continue;
		}
		
		pthread_mutex_lock(&Locker);
		while(counter == MAXPAR) pthread_cond_wait(&Vcond, &Locker);
		pthread_mutex_unlock(&Locker);
		pid = fork();
		if (pid == -1) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) {
			/* Child process */
			sprintf(title,"par-shell-out-%d.txt", getpid());
			fchild = open(title, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
			close(1);
			dup(fchild);
			close(fchild);
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
	
	killTerminal();
	pthread_join(monitorthread, NULL);
	close(fpipe);
	close(fstats);
	unlink("/tmp/par-shell-in");
	unlink("/tmp/stats");
	pthread_mutex_destroy(&Locker);
	pthread_cond_destroy(&Vcond);
	pthread_cond_destroy(&Sleep);
	lst_destroy(list);
	fclose(myfile);
	return(0);
}
