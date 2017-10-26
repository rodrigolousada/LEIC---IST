/* TCS */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#define TCSPORT 58067
#define MAX 500
#define MAX_L 99
#define MAX_CHAR 100
#define STDIN 0

/* Structures */
typedef struct language_list {
	int TRSport;
	char *name, *TRSip;
	struct language_list* next;
} Language;

typedef Language* Language_link;


/* Prototipes */
Language_link insertLanguage(Language_link head, int TRSport, char* name, char* TRSip);
Language_link delete(Language_link head, char* name, char* TRSip, int TRSport);
int gracefulexit(char* str);
Language_link confirmLanguageExistance(Language_link head, char* language, int TRSport);

int main(int argc, char **argv) {
	
	/* Variables */
	fd_set fds;
	int i, fd, TCSport, TRSport;
	struct hostent *hostptr;
	struct sockaddr_in serveraddr, clientaddr;
	int addrlen, maxfd, n_languages=0;
	char *tokens, *TRSip, *language_name, command[MAX_CHAR], buffer[MAX], msg[MAX], status[3];
	Language_link head=NULL, last=NULL, aux;

	
	/* Analizing Flags */
	TCSport=TCSPORT;
	if(argc!=1 && argc!=3) {
		gracefulexit("error: invalid arguments");
	}
	if(argc>2) {
		if(strcmp(argv[1],"-p")==0){
			TCSport=atoi(argv[2]);
		}
		else {
			gracefulexit("error: invalid arguments");
		}
	}
	
	/* User & TRS UDP Server */
	
	if((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1) {
		printf("error: creating UDP socket\n");
		exit(1);
	}
	
	memset((void*)&serveraddr, (int)'\0',sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddr.sin_port=htons((u_short)TCSport);
	
	if(bind(fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))==-1){
		printf("error: bind function\n");
		exit(1);
	}
    
	while(1) {
		FD_ZERO(&fds);
		FD_SET(fd, &fds); 
		FD_SET(STDIN, &fds);
		maxfd = (fd > STDIN)?fd:STDIN;
		select(maxfd+1, &fds, NULL, NULL, NULL); 
		
		if (FD_ISSET(STDIN, &fds)){
			fgets(command, 128, stdin);
			tokens = strtok(command, " \n");
			if(strcmp(command,"exit")==0){
				break;
			}
		}
		if (FD_ISSET(fd, &fds)){
			addrlen = sizeof(clientaddr);
			memset(msg, 0, MAX);
			memset(buffer, 0, MAX);
			memset(status, 0, 3);
			strcpy(status, "OK");
			recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &addrlen);
			
			tokens = strtok (buffer," \n");
			
			if(strcmp(tokens,"ULQ")==0) { 
				strcpy(msg, "ULR");
				if(n_languages==0) {
					strcat(msg, " EOF");
				}
				else {
					printf("List request\n");
					sprintf(msg, "%s %d", msg, n_languages);
					for(aux=head; aux!=NULL; aux=aux->next) {
						sprintf(msg, "%s %s", msg, aux->name);
					}
				}
				if((tokens=strtok(NULL, " \n"))!=NULL) strcpy(msg, "ULR ERR");
			}
			else if (strcmp(tokens, "UNQ")==0) {
				if((tokens = strtok(NULL, " \n"))==NULL) {
					printf("error: unq invalid command received");
					strcpy(msg, "ULR ERR\n");
					sendto(fd,msg,strlen(msg),0,(struct sockaddr*)&clientaddr, addrlen);
					continue;
				}
				if((aux=confirmLanguageExistance(head, tokens, -1))==NULL) strcpy(msg, "UNR EOF");
				else sprintf(msg, "UNR %s %d", aux->TRSip, aux->TRSport);
			}
			else if(strcmp(tokens,"SRG")==0) {
				if((tokens = strtok(NULL, " \n"))==0){
					printf("error: srg invalid command received");
					strcpy(msg, "SRR ERR\n");
					sendto(fd,msg,strlen(msg),0,(struct sockaddr*)&clientaddr, addrlen);
					continue;
				}
				language_name = tokens; //register language name
				tokens = strtok(NULL, " \n");
				TRSip = tokens; //registers TRS ip
				tokens = strtok(NULL, " \n");
				TRSport = atoi(tokens); //registers TRS port
				aux=confirmLanguageExistance(head, language_name, TRSport);
				if(aux==NULL) {
					head = insertLanguage(head, TRSport, language_name, TRSip); 
					printf("+%s %s %d\n", language_name, TRSip, TRSport);
					n_languages++;
					sprintf(msg, "SRR %s", status);
				}
				else {
					strcpy(msg,"SRR NOK");
				}
			}
			else if(strcmp(tokens, "SUN")==0) {
				if((tokens = strtok(NULL, " \n"))==0){
					printf("error: srg invalid command received");
					strcpy(msg, "SUR ERR\n");
					sendto(fd,msg,strlen(msg),0,(struct sockaddr*)&clientaddr, addrlen);
					continue;
				}
				if((aux=confirmLanguageExistance(head, tokens, -1))==NULL) strcpy(status, "NOK"); //confirm language existence
				tokens = strtok(NULL, " \n");
				TRSip = tokens;
				tokens = strtok(NULL, " \n");
				TRSport = atoi(tokens); //registers TRS port
				if(strcmp(aux->TRSip,TRSip)!=0 || aux->TRSport!=TRSport) strcpy(status,"ERR");
				else if(strcmp(status,"OK")==0) {
					printf("-%s %s %d\n", aux->name, aux->TRSip, aux->TRSport);
					head = delete(head, aux->name, aux->TRSip, aux->TRSport);
					n_languages--;
				}
				sprintf(msg, "SUR %s", status);
			}
			else {
				printf("error: invalid command");
				continue;
			}
			strcat(msg, "\n");
			sendto(fd,msg,strlen(msg),0,(struct sockaddr*)&clientaddr, addrlen);
		}
	}
	
	/* Cleaning memory */
	for(aux=head; aux!=NULL; aux=aux->next) {
		if(aux==head) head = head->next;
		else last->next = aux->next;
		free(aux->TRSip);
		free(aux->name);
		free(aux);
	}
	
	close(fd);
	
	return 0;
}


/* Auxiliar Functions */
Language_link insertLanguage(Language_link head, int TRSport, char* name, char* TRSip) {
	Language_link aux;
	Language_link new = (Language_link) malloc(sizeof(Language));
	
	new->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
	new->TRSip = (char*)malloc(sizeof(char)*(strlen(TRSip)+1));
	
	new->TRSport = TRSport;
	strcpy(new->name, name);
	strcpy(new->TRSip, TRSip);
	new->next = NULL;
	
	if(head==NULL) {
		head=new;
	}
	else {
		for(aux=head; aux->next!=NULL; aux=aux->next);
		aux->next=new;
	}
	
	return head;
}

Language_link delete(Language_link head, char* name, char* TRSip, int TRSport) {
	Language_link aux, last;
	for(aux = head, last=NULL; aux!=NULL; last=aux, aux=aux->next) {
		if(strcmp(aux->name, name)==0 && strcmp(aux->TRSip,TRSip)==0 && aux->TRSport==TRSport) {
			if(aux==head) head = head->next;
			else last->next = aux->next;
			free(aux->TRSip);
			free(aux->name);
			free(aux);
		}	
	}
	return head;
}

Language_link confirmLanguageExistance(Language_link head, char* language, int TRSport) {
	Language_link aux, res=NULL;
	
	for(aux=head; aux!=NULL; aux=aux->next) {
		if(strcmp(aux->name, language)==0 || TRSport==aux->TRSport) {
			res=aux;
		}
	}
	
	return res;
}

int gracefulexit(char* str) {	
	if(str!=NULL)printf("%s\n", str);
	exit(1);
}
