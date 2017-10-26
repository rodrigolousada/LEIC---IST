/* TRS */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#define TRSPORT 59000
#define TCSPORT 58067
#define MAX 500
#define MAX_CHAR 100
#define CLIENT_LIMIT 5
#define STDIN 0

int gracefulexit(char* str) {
	if(str!=NULL)printf("%s\n", str);
	exit(1);
}

int main(int argc, char **argv) {
	
	/* Variables */
	FILE *fp, *fp_files, *fp_image;
	fd_set fds;
	int abort = 0, fd_tcs, fd_user, maxfd, newfd_user, i, i2, n_words, addrlen, clientlen, TRSport, size_file, nleft, nwritten, nread, TCSport;
	char *TCSname, *tokens, *buffer3, *tokensfile, *TRSname, filename[MAX_CHAR], filename2[MAX_CHAR], filename3[MAX_CHAR], filename_traduced[MAX_CHAR], TRSip[MAX], command[MAX_CHAR], answer[MAX], msg[MAX], buffer[MAX], buffer2[MAX], hostname[MAX], line[MAX];
	char *language;
	struct hostent *hostptr;
	struct sockaddr_in serveraddr_tcs, serveraddr, clientaddr;
	
	struct hostent *h;
	struct in_addr *a;

	/* Analizing Flags */
	language=argv[1];
	TCSport=TCSPORT;
	TRSport=TRSPORT;
	if(gethostname(hostname,128)==-1)
		gracefulexit("error: detecting hostname");
	TCSname=TRSname=&hostname[0];
	
	
	if(argc!=2 && argc!=4 && argc!=6 && argc!=8) {
		gracefulexit("error: invalid arguments");
	}
	
	if(argc>=3) {
		for(i=2; i<argc; i+=2) {
			if(strcmp(argv[i],"-n")==0) {
				TCSname=argv[i+1];
			}
			else if(strcmp(argv[i],"-e")==0){
				TCSport = atoi(argv[i+1]);
			}
			else if(strcmp(argv[i],"-p")==0){
				TRSport = atoi(argv[i+1]);
			}
			else {
				gracefulexit("error: invalid arguments");
			}
		}
	}
	
	h=gethostbyname(TRSname);	
	a = (struct in_addr*)h->h_addr_list[0];	

	/* User TCP Server */	
	if((fd_user = socket(AF_INET, SOCK_STREAM, 0))==-1) {
		printf("error: creating TCP socket\n");
		exit(1);
	}
	
	memset((void *)&serveraddr,(int)'\0',sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddr.sin_port=htons((u_short)TRSport);
	
	strcpy(TRSip,inet_ntoa(*a));
	
	if(bind(fd_user, (struct sockaddr*)&serveraddr, sizeof(serveraddr))==-1) {
		printf("error: bind function\n");
		close(fd_user);
		exit(1);
	}
		
	if(listen(fd_user,CLIENT_LIMIT)==-1) {
		printf("error: listen function\n");
		close(fd_user);
		exit(1);
	}
		
	
	/* TCS UDP Client */
	if((fd_tcs=socket(AF_INET,SOCK_DGRAM,0))==-1) {
		printf("error: creating UDP socket\n");
		close(fd_user);
		exit(1);
	}
	
	hostptr=gethostbyname(TCSname);
	
	memset((void*)&serveraddr_tcs,(int)'\0',sizeof(serveraddr_tcs));
	serveraddr_tcs.sin_family=AF_INET;
	serveraddr_tcs.sin_addr.s_addr=((struct in_addr *) (hostptr->h_addr_list[0]))->s_addr;
	serveraddr_tcs.sin_port=htons((u_short)TCSport);	
	
	addrlen=sizeof(serveraddr_tcs);
	sprintf(msg, "SRG %s %s %d\n", language, TRSip, TRSport);
	sendto(fd_tcs,msg,strlen(msg),0,(struct sockaddr*) &serveraddr_tcs, addrlen);
	
	addrlen=sizeof(serveraddr_tcs);
	memset(buffer, 0, MAX);
	if(recvfrom(fd_tcs, buffer, sizeof(buffer),0,(struct sockaddr*)&serveraddr_tcs, &addrlen)==-1) {
		printf("error: receiving from TCS\n");
		close(fd_tcs);
		close(fd_user);
		exit(1);
	}
    
	printf("%s", buffer);
	
	if(strcmp(buffer,"SRR OK\n")!=0){
		printf("error: registing in TCS\n");
		close(fd_tcs);
		close(fd_user);
		exit(1);
	}
	
	/* Open translation file */
	sprintf(filename, "text_translation_%s.txt", language);
	if((fp = fopen(filename, "r"))==NULL) {
		printf("error: language not available - opening file\n");
		abort = 1;
	}
	sprintf(filename2, "file_translation_%s.txt", language);
	if((fp_files = fopen(filename2, "r"))==NULL) {
		printf("error: language not available - opening file\n");
		abort = 1;
	}
	
	while(1) {
		if(abort==1)break;
		FD_ZERO(&fds);
		FD_SET(fd_user, &fds); 
		FD_SET(STDIN, &fds);
		maxfd = (fd_user > STDIN)?fd_user:STDIN;
		select(maxfd+1, &fds, NULL, NULL, NULL); 
		
		if (FD_ISSET(STDIN, &fds)){
			fgets(command, 128, stdin);
			tokens = strtok(command, " \n");
			if(strcmp(command,"exit")==0){
				break;
			}
		}
		if (FD_ISSET(fd_user, &fds)){
			
			clientlen=sizeof(clientaddr);
			if((newfd_user=accept(fd_user,(struct sockaddr*)&clientaddr,&clientlen))==-1) {
				printf("error: accept function\n");
				break;
			}
			
			memset(msg, 0, MAX);
			memset(buffer, 0, MAX);
			memset(buffer2, 0, MAX);
			read(newfd_user,buffer,MAX);
			
			
			if(strcmp(buffer,"\0")==0) { continue; }
			
			strcpy(buffer2, buffer);
			
			tokens=strtok(buffer, " \n");
			if(strcmp("TRQ",tokens)==0) {
				tokens=strtok(NULL, " \n");
				if(strcmp("t",tokens)==0) {
					if((tokens=strtok(NULL, " \n"))==NULL) {
						printf("error: invalid request\n");
						strcpy(msg,"TRR ERR\n");
						write(newfd_user, msg, strlen(msg));
						continue;
					}
					if((n_words=atoi(tokens))==0) {
						printf("error: atoi function - invalid request\n");
						strcpy(msg,"TRR ERR\n");
						write(newfd_user, msg, strlen(msg));
						continue;
					}
					if(n_words==0) strcpy(answer, "TRR ERR");
					else {
						sprintf(answer, "TRR t %d", n_words);
						for(i=0; i<n_words; i++) {
							rewind(fp);
							strcpy(buffer, buffer2);
							
							for(tokens=strtok(buffer, " \n"), i2=0; i2<i+3 ; i2++, tokens=strtok(NULL, " \n")); //voltar a ter o tokens que queremos
							
							while(1) {
								if(fgets(line, MAX_CHAR, fp)==NULL) {
									printf("Translation request for word that we are not able to attend!\n");
									strcpy(answer,"TRR NTA");
									break;
								}
								tokensfile=strtok(line, " \n");
								if(strcmp(tokens, tokensfile)==0) {
									sprintf(answer, "%s %s", answer, strtok(NULL, " \n"));
									break;
								}
							}
						}
					}
					sprintf(msg, "%s\n", answer);
				}
				else if(strcmp("f",tokens)==0) {
					if((tokens=strtok(NULL, " \n"))==NULL) {
						printf("error: invalid command\n");
						close(newfd_user);
						continue;
					}
					strcpy(filename, tokens);
					sprintf(filename2, "files/%s" , filename);
					
					if((fp_image = fopen(filename2,"wb"))==NULL) {
						printf("error: no such filename\n");
						close(newfd_user);
						break;
					}
					
					if((tokens=strtok(NULL, " \n"))==NULL) {
						printf("error: invalid command\n");
						close(newfd_user);
						continue;
					}
					if((size_file=atoi(tokens))==0){
						printf("error: atoi function - invalid answer\n");
						close(newfd_user);
						continue;
					}
					
					nleft=size_file;
					tokens=(char*)malloc(sizeof(tokens)*(size_file+1));
					buffer3 = (char*)malloc(sizeof(buffer3)*(size_file+1));
					
					
					if((tokens=strtok(NULL, ""))!=NULL) {
						fwrite(tokens, 1, strlen(tokens), fp_image);
						nleft -= strlen(tokens);
					}
					while(nleft > 0) {
						nread=read(newfd_user, buffer3, nleft);
						fwrite(buffer3, 1, nread, fp_image);
						if(nread==-1) {
							printf("error: reading image data\n");
							fclose(fp_image);
							break;
						}
						else if(nread==0)break;
						nleft-=nread;
					}
					if(fp_image==NULL) {
						printf("error: writting file\n");
						continue;
					}
					
					free(tokens);
					free(buffer3);
					fclose(fp_image);
					printf("A traduzir:%s\n", filename);
					
					
					//Search for filename translation
					while(1) {
						if(fgets(line, MAX_CHAR, fp_files)==NULL) {
							printf("Translation request for word that we are not able to attend!\n");
							strcpy(msg,"TRR NTA");
							break;
						}
						tokensfile=strtok(line, " \n");
						
						if(strcmp(tokensfile, filename)==0) {
							strcpy(filename_traduced, strtok(NULL, " \n"));
							break;
						}
					}
					
					if(strcmp(msg,"TRR NTA")==0) break;
					
					//Send file to user
					sprintf(filename3, "files/%s", filename_traduced);				
					if((fp_image = fopen(filename3,"rb"))==NULL) {
						printf("error: no such filename\n");
						close(newfd_user);
						break;
					}
					
					fseek(fp_image, 0, SEEK_END);
					size_file=ftell(fp_image);
					fseek(fp_image, 0, SEEK_SET);
					
					sprintf(msg, "TRR f %s %d " ,filename_traduced, size_file);
					write(newfd_user, msg, strlen(msg));
					
					while(size_file > 0) {
						nwritten = write(newfd_user, fp_image, size_file);
						if(nwritten<=0) {
							printf("error: nwritten problem\n"); 
							break; 
						}
						size_file-=nwritten;
						fseek(fp_image, nwritten, SEEK_CUR);
					}
					
					strcpy(msg,"\n");
					fclose(fp_image);
						
				}
		
			}
			else strcpy(msg, "TRR ERR\n");			
			
			write(newfd_user, msg, strlen(msg));
		}
	}
	
	addrlen=sizeof(serveraddr_tcs);
	
	sprintf(msg, "SUN %s %s %d", language, TRSip, TRSport);
	sendto(fd_tcs,msg,strlen(msg),0,(struct sockaddr*) &serveraddr_tcs, addrlen);
	
	addrlen=sizeof(serveraddr_tcs);
	if(recvfrom(fd_tcs, buffer, sizeof(buffer),0,(struct sockaddr*)&serveraddr_tcs, &addrlen)==-1) exit(1);
	if(strcmp(buffer,"SUR OK\n")!=0) {
		printf("error: problems exiting from tcs\n");
	}
	else printf("Exit Success\n");
	
	/* Closing connection */
	if(abort==0) {
		fclose(fp);
		fclose(fp_files);
		close(newfd_user);
	}
	close(fd_tcs);
	close(fd_user);
	return 0;
}
