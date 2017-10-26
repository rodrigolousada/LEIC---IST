/* USER */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#define TCSPORT 58067
#define MAX 500

void gracefulexit(char* str) {
	if(str!=NULL)printf("%s\n", str);
	exit(1);
}

//Function that cleans scanf
void discardscanf(){
	char c;
	while((c=getchar()) != '\n' && c != EOF);
}

int main(int argc, char **argv) {
	
	/* Variables */
	FILE *fp;
	int i, n, *fd, fd_tcs, fd_trs, n_languages=0, addrlen, TCSport, TRSport, language, size_file, size_extra, nleft, nread, nwritten;
	char *TCSname, *tokens, *ptr, *buffer2, languages[MAX][MAX], TRSip[MAX], buffer[MAX], msg[MAX], msg2[MAX], filename[MAX],hostname[MAX];
	struct hostent *hostptr, *hostptr2;
	struct sockaddr_in serveraddr_tcs, serveraddr_trs, clientaddr;
	
	/* Analizing Flags */
	TCSport=TCSPORT;
	if(gethostname(hostname,128)==-1)
		gracefulexit("error: detecting hostname");
	TCSname=&hostname[0];
	
	if(argc!=1 && argc!=3 && argc!=5) {
		gracefulexit("error: invalid arguments");
	}
	
	if(argc>=2) {
		for(i=1; i<argc; i+=2) {
			if(strcmp(argv[i],"-n")==0) {
				TCSname=argv[i+1];
			}
			else if(strcmp(argv[i],"-p")==0) {
				TCSport = atoi(argv[i+1]);
			}
			else {
				gracefulexit("error: invalid arguments");
			}
		}
	}
	
	/* TCS UDP Client */
	if((fd_tcs=socket(AF_INET,SOCK_DGRAM,0))==-1) {
		printf("error: creating UDP socket\n");
		exit(1);
	}
	
	hostptr=gethostbyname(TCSname);
	
	memset((void*)&serveraddr_tcs,(int)'\0',sizeof(serveraddr_tcs));
	serveraddr_tcs.sin_family=AF_INET;
	serveraddr_tcs.sin_addr.s_addr=((struct in_addr *) (hostptr->h_addr_list[0]))->s_addr;
	serveraddr_tcs.sin_port=htons((u_short)TCSport);
	
	while(1) {
		addrlen=sizeof(serveraddr_tcs);
		memset(msg, 0, MAX);
		memset(msg2, 0, MAX);
		memset(buffer, 0, MAX);
		if(scanf("%[^\n]%*c", msg)!=1){
			printf("error: no input detected\n");
			discardscanf();
			continue;
		}
		strcpy(msg2, msg); //backup string before strtok command
		tokens = strtok(msg," \n");
		//EXIT
		if(strcmp(msg2,"exit")==0) {
			break;
		}
		//LIST
		else if(strcmp(msg2,"list")==0) {
			strcpy(msg,"ULQ\n");
			
			sendto(fd_tcs,msg,strlen(msg),0,(struct sockaddr*) &serveraddr_tcs, addrlen); //ULQ message
			addrlen=sizeof(serveraddr_tcs);
			if(recvfrom(fd_tcs, buffer, sizeof(buffer),0,(struct sockaddr*)&serveraddr_tcs, &addrlen)==-1)exit(1); //ULR n L1 L2 ... Ln
			
			tokens = strtok(buffer," \n");
			if(strcmp(tokens,"ULR")!=0) {
				printf("error: invalid answer\n");
				break;
			}
			tokens = strtok(NULL, " \n");
			if(strcmp(tokens, "EOF")==0) printf("No languages available :(\n");
			else if(strcmp(tokens, "ERR")==0) {
				printf("error: ULQ request\n");
				break;
			}
			else {
				if((n_languages=atoi(tokens))==0) {
					printf("error: atoi function - invalid answer\n");
					break;
				}
				for(i=0; i<n_languages && tokens!=NULL; i++) {
					tokens=strtok(NULL, " \n");
					printf(" %d - %s\n", i+1, tokens);
					strcpy(languages[i+1],tokens);
				}
				if(i<n_languages) { //if stoped by tokens!=NULL
					printf("error: invalid answer\n");
					break;
				}
			}
		}
		//REQUEST
		else if(strcmp(tokens,"request")==0) {
			if((tokens = strtok(NULL, " "))==NULL) {
				printf("error: invalid request\n");
				continue;
			}
			if((language=atoi(tokens))==0) {
				printf("error: atoi function - invalid request\n");
				continue;
			}
			if(language > n_languages || language==0) {
				printf("error: invalid command\n");
				continue;
			}
			
			
			//request TCS for TRS's IP and Port
			sprintf(msg, "UNQ %s\n", languages[language]);
			
			sendto(fd_tcs,msg,strlen(msg),0,(struct sockaddr*) &serveraddr_tcs, addrlen); //UNQ Ln
			addrlen=sizeof(serveraddr_tcs);
			memset(msg, 0, MAX);
			memset(buffer, 0, MAX);
			if(recvfrom(fd_tcs, buffer, sizeof(buffer),0,(struct sockaddr*)&serveraddr_tcs, &addrlen)==-1)exit(1); //UNR IPTRS portTRS
			
			tokens = strtok(buffer, " \n");
			if(strcmp(tokens,"UNR")!=0) {
				printf("error: invalid answer\n");
				break;
			}
			tokens = strtok(NULL, " \n");
			if(strcmp(tokens, "EOF")==0) {
				printf("That language is not available!\n");
				continue;
			}
			else if(strcmp(tokens, "ERR")==0) {
				printf("error: command not available\n");
				continue;
			}
			strcpy(TRSip,tokens);
			tokens = strtok(NULL, " \n");
			if((TRSport = atoi(tokens))==0) {
				printf("error: atoi function - invalid answer\n");
				break;
			}
			
			printf("%s %d\n", TRSip, TRSport);
				
			//Stablishing connection via TCP with TSR
			if((fd_trs = socket(AF_INET, SOCK_STREAM, 0))==-1) {
				printf("error: creating TCP socket\n");
				break;
			}
				
			hostptr2 = gethostbyname(TRSip);
				
			memset((void*)&serveraddr_trs,(int)'\0',sizeof(serveraddr_trs));
			serveraddr_trs.sin_family = AF_INET;
			serveraddr_trs.sin_addr.s_addr = ((struct in_addr *)(hostptr2->h_addr_list[0]))->s_addr;
			serveraddr_trs.sin_port = htons((u_short)TRSport);
				
			if((connect(fd_trs, (struct sockaddr*)&serveraddr_trs, sizeof(serveraddr_trs)))==-1) { 
				printf("error: connect tcp user/tsr\n");
				close(fd_trs);
				break;
			}
			
			for(i=0, tokens = strtok(msg2," "); i<2 ; tokens=strtok(NULL, " "), i++); //redo strtok
			if(tokens==NULL) {
				printf("error: invalid command\n");
				close(fd_trs);
				continue;
			}
			else if(strcmp(tokens, "t")==0) {
				if((tokens=strtok(NULL, " "))==NULL) {
					printf("error: invalid command\n");
					close(fd_trs);
					continue;
				}
				strcpy(msg2,tokens);
				for(i=1, tokens=strtok(NULL, " "); tokens!=NULL; tokens=strtok(NULL, " "), i++) {
					sprintf(msg2, "%s %s", msg2, tokens);
				}
				sprintf(msg, "TRQ t %d %s\n" , i, msg2);
			
				write(fd_trs, msg, strlen(msg));
			}
			else if(strcmp(tokens,"f")==0) { //to correct
				if((tokens=strtok(NULL, " "))==NULL) {
					printf("error: invalid command\n");
					close(fd_trs);
					continue;
				}
				strcpy(filename, tokens);
				
				if((fp = fopen(filename,"rb"))==NULL) {
					printf("error: no such filename\n");
					close(fd_trs);
					continue;
				}
				
				fseek(fp, 0, SEEK_END);
				size_file=ftell(fp);
				fseek(fp, 0, SEEK_SET);
				
				//send file (probably the problem is here)
				sprintf(msg, "TRQ f %s %d " ,filename, size_file);
				write(fd_trs, msg, strlen(msg));
				
				while(size_file > 0) {
					nwritten = write(fd_trs, fp, size_file);
					if(nwritten<=0) { 
						printf("error: nwritten problem\n"); 
						break; 
					}
					size_file-=nwritten;
					fseek(fp, nwritten, SEEK_CUR);
				}
				
				write(fd_trs,"\n",strlen("\n"));
				fclose(fp);
			}
			else {
				printf("error: invalid command\n");
				close(fd_trs);
				continue;
			}
			
			memset(msg, 0, MAX);
			memset(buffer, 0, MAX);
			
			read(fd_trs, buffer, MAX);
			
			tokens = strtok(buffer, " \n");
			if(strcmp(tokens, "TRR")!=0) {
				printf("error: invalid answer\n");
				close(fd_trs);
				break;
			}
			tokens = strtok(NULL, " \n");
			if(strcmp(tokens,"ERR")==0) {
				printf("error: TRQ request\n");
				close(fd_trs);
				break;
			}
			else if(strcmp(tokens,"NTA")==0) printf("%s: No translation available\n", TRSip);
			else if(strcmp(tokens,"t")==0) {
				tokens=strtok(NULL, " \n");
				if((n = atoi(tokens))==0) {
					printf("error: atoi function - invalid answer\n");
					close(fd_trs);
					break;
				}
				sprintf(msg, "%s:", TRSip);
				for(i=0, tokens=strtok(NULL, " \n"); i<n && tokens!=NULL; i++,tokens=strtok(NULL, " \n")) {
					sprintf(msg, "%s %s", msg, tokens);
				}
				if(i<n){
					printf("error: invalid answer\n");
					close(fd_trs);
					break;
				}
				printf("%s\n", msg);
			}
			else if(strcmp(tokens,"f")==0) {
				if((tokens=strtok(NULL, " \n"))==NULL) {
					printf("error: invalid command\n");
					close(fd_trs);
					continue;
				}
				strcpy(filename, tokens);
				
				if((fp = fopen(filename,"wb"))==NULL) {
					printf("error: no such filename\n");
					continue;
				}
				
				if((tokens=strtok(NULL, " \n"))==NULL) {
					printf("error: invalid command\n");
					close(fd_trs);
					continue;
				}
				if((size_file=atoi(tokens))==0){
					printf("error: atoi function - invalid answer\n");
					close(fd_trs);
					continue;
				}
				
				nleft=size_file;
				tokens=(char*)malloc(sizeof(tokens)*(size_file+1));
				buffer2 = (char*)malloc(sizeof(buffer2)*(size_file+1));
				
				
				if((tokens=strtok(NULL, ""))!=NULL) {
					fwrite(tokens, 1, strlen(tokens), fp);
					nleft -= strlen(tokens);
				}
				while(nleft > 0) {
					nread=read(fd_trs, buffer2, nleft);
					fwrite(buffer2, 1, nread, fp);
					if(nread==-1) {
						printf("error: reading image data\n");
						fclose(fp);
						break;
					}
					else if(nread==0)break;
					nleft-=nread;
				}
				if(fp==NULL) {
					printf("error: writting file\n");
					continue;
				}
				
				free(tokens);
				fclose(fp);
				//free(buffer2);
			}
			else {
				printf("error: invalid answer\n");
				close(fd_trs);
				break;
			}
			
			close(fd_trs);
		}
		else {
			printf("error: invalid command\n");
		}
	}
	
	close(fd_tcs);
	
	return 0;
}

