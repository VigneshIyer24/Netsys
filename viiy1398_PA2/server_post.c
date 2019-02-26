#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define BUFSIZE 1000000
#define MAX_COUNT  200

void error(char *msg) {
  perror(msg);
  exit(1);
}
int main(int argc, char **argv) {

	int sockfd; /* socket */
	int portno; /* port to listen on */
	int clientlen; /* byte size of client's address */
	struct sockaddr_in serveraddr; /* server's addr */
	struct sockaddr_in clientaddr; /* client addr */
	struct hostent *hostp; /* client host info */
  	char buf[BUFSIZE]; /* message buf */
	char sendback[BUFSIZE]; /*For index or default page*/
	char cssbuf[BUFSIZE];	/*For css folder*/
	char file1[BUFSIZE];
	char filename[100]; /*For opening a specfic file in a folder*/
	char *hostaddrp; /* dotted decimal host addr string */
	int optval; /* flag value for setsockopt */
	int n; /* message byte size */
   	char *check;
	char *file_path;
	char *protocol;
	char *format;
	char *timepass;
	char *indice;
	char length_file[10];
	pid_t pid; 
	int max;
	int fd;
	int readfile;
	char header_1[100];

	/* 
   	* check command line arguments 
   	*/
  	if (argc != 2) {
    		fprintf(stderr, "usage: %s <port>\n", argv[0]);
    		exit(1);
  	}
  	portno = atoi(argv[1]);

  	/* 
   	* socket: create the parent socket 
   	*/
  	sockfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (sockfd < 0) 
    		error("ERROR opening socket");

  	/* setsockopt: Handy debugging trick that lets 
   	* us rerun the server immediately after we kill it; 
   	* otherwise we have to wait about 20 secs. 
   	* Eliminates "ERROR on binding: Address already in use" error. 
   	*/
  	optval = 1;
  	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     	(const void *)&optval , sizeof(int));

  	/*
   	* build the server's Internet address
   	*/
  	bzero((char *) &serveraddr, sizeof(serveraddr));
  	serveraddr.sin_family = AF_INET;
  	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	serveraddr.sin_port = htons((unsigned short)portno);

  	/* 
   	* bind: associate the parent socket with a port 
   	*/
  	if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   	sizeof(serveraddr)) < 0) 
    	error("ERROR on binding");

	clientlen = sizeof(clientaddr);
	n=listen(sockfd,100000);
	int i;
	bzero(buf,10);
	void setTimeout(int milliseconds);
	printf("Waiting for connection\n");
	for( ; ; )
	{
		i=10000;
		fd=accept(sockfd,(struct sockaddr *)&clientaddr,&clientlen);
		if(fd < 0)
		{
			error("INvalid error");
			exit(1);
		}
		pid=fork();
		if(pid==0)
		{
			
			n=recv(fd,buf,100000,0);
			buf[n] = '\0';
			if(n<0)
				error("ERROR in recvfrom");
			printf("%s\n",buf);
		//bzero(buf,100000);
			bzero(filename,100);
			check=strtok(buf," ");
			file_path = strtok(NULL," ");
			protocol = strtok(NULL, " ");
			strcat(filename,"www");
			strcat(filename,file_path);
			printf("%s\n",filename);
			format=strchr(file_path,'.');
			printf("%s\n",format);
			if(!(strcmp(check,"GET")))
			{
				if(!(strcmp(format,".html")))
				{
					
					bzero(file1,BUFSIZE);
		                        bzero(length_file,10);
					bzero(header_1,20);						
					FILE *fp1;
					fp1=fopen(filename,"rb");
					fseek(fp1,0,SEEK_END);
					max=ftell(fp1);
		                	sprintf(length_file,"%d",max);
		                	fseek(fp1,0,SEEK_SET);
					sprintf(header_1,"HTTP/1.1 200 Ok\r\nContent-type: text/html\r\nContent-length: %d\r\n\r\n",max);
					printf("%s\n",header_1);				
					fread(file1,1,max,fp1);
					fclose(fp1);
					n=send(fd,header_1,strlen(header_1),0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
					n=send(fd,file1,max,0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
				}
				else if(!(strcmp(format,".txt")))
				{
					bzero(file1,BUFSIZE);
		                        bzero(length_file,10);
					bzero(header_1,20);						
					FILE *fp1;
					fp1=fopen(filename,"r");
					fseek(fp1,0,SEEK_END);
					max=ftell(fp1);
		                	sprintf(length_file,"%d",max);
		                	fseek(fp1,0,SEEK_SET);
					sprintf(header_1,"HTTP/1.1 200 Ok\r\nContent-type: text/plain\r\nContent-length: %d\r\n\r\n",max);
					printf("%s\n",header_1);				
					fread(file1,1,max,fp1);
					fclose(fp1);
					n=send(fd,header_1,strlen(header_1),0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
					n=send(fd,file1,max,0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
						
				}
				else if(!(strcmp(format,".css")))
				{
					bzero(file1,BUFSIZE);
		                        bzero(length_file,10);
					bzero(header_1,20);						
					FILE *fp1;
					fp1=fopen(filename,"r");
					fseek(fp1,0,SEEK_END);
					max=ftell(fp1);
		                	sprintf(length_file,"%d",max);
		                	fseek(fp1,0,SEEK_SET);
					sprintf(header_1,"HTTP/1.1 200 Ok\r\nContent-type: text/css\r\nContent-length: %d\r\n\r\n",max);
					printf("%s\n",header_1);				
					fread(file1,1,max,fp1);
					fclose(fp1);
					n=send(fd,header_1,strlen(header_1),0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
					n=send(fd,file1,max,0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
				
				}
				else if(!(strcmp(format,".jpg")))
				{
					bzero(file1,BUFSIZE);
		                        bzero(length_file,10);
					bzero(header_1,20);						
					FILE *fp1;
					fp1=fopen(filename,"rb");
					fseek(fp1,0,SEEK_END);
					max=ftell(fp1);
		                	sprintf(length_file,"%d",max);
		                	fseek(fp1,0,SEEK_SET);
					sprintf(header_1,"HTTP/1.1 200 Ok\r\nContent-type: image/jpg\r\nContent-length: %d\r\n\r\n",max);
					printf("%s\n",header_1);				
					fread(file1,1,max,fp1);
					fclose(fp1);
					n=send(fd,header_1,strlen(header_1),0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
					n=send(fd,file1,max,0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
				
				}
				else if(!(strcmp(format,".png")))
				{
					bzero(file1,BUFSIZE);
		                        bzero(length_file,10);
					bzero(header_1,20);						
					FILE *fp1;
					fp1=fopen(filename,"rb");
					fseek(fp1,0,SEEK_END);
					max=ftell(fp1);
		                	sprintf(length_file,"%d",max);
		                	fseek(fp1,0,SEEK_SET);
					sprintf(header_1,"HTTP/1.1 200 Ok\r\nContent-type: image/jpg\r\nContent-length: %d\r\n\r\n",max);
					printf("%s\n",header_1);				
					fread(file1,1,max,fp1);
					fclose(fp1);
					n=send(fd,header_1,strlen(header_1),0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
					n=send(fd,file1,max,0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
				
				}
				else if(!(strcmp(format,".gif")))
				{
					bzero(file1,BUFSIZE);
		                        bzero(length_file,10);
					bzero(header_1,20);						
					FILE *fp1;
					fp1=fopen(filename,"rb");
					fseek(fp1,0,SEEK_END);
					max=ftell(fp1);
		                	sprintf(length_file,"%d",max);
		                	fseek(fp1,0,SEEK_SET);
					sprintf(header_1,"HTTP/1.1 200 Ok\r\nContent-type: image/jpg\r\nContent-length: %d\r\n\r\n",max);
					printf("%s\n",header_1);				
					fread(file1,1,max,fp1);
					fclose(fp1);
					n=send(fd,header_1,strlen(header_1),0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
					n=send(fd,file1,max,0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
				
				}
				else 
				{
					bzero(file1,BUFSIZE);
		                        bzero(length_file,10);
					bzero(header_1,20);						
					FILE *fp1;
					fp1=fopen(filename,"r");
					fseek(fp1,0,SEEK_END);
					max=ftell(fp1);
		                	sprintf(length_file,"%d",max);
		                	fseek(fp1,0,SEEK_SET);
					sprintf(header_1,"HTTP/1.1 200 Ok\r\nContent-type: application/javascript\r\nContent-length: %d\r\n\r\n",max);
					printf("%s\n",header_1);				
					fread(file1,1,max,fp1);
					fclose(fp1);
					n=send(fd,header_1,strlen(header_1),0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
					n=send(fd,file1,max,0);
						if(n<0)
						{
							error("ERROR in recvfrom");
						exit(1);
						}
				
				}
				close(fd);
			}
			else if(!(strcmp(check,"POST")))
			{
				printf("HELLO\n");
				close(fd);
				exit(0);
			}	
		
		}
		else
		{
			bzero(header_1,20);			
			printf("HTTP/1.1 500 Internal Server Error\n");
			sprintf(header_1,"HTTP/1.1 500 Internal Server Error\n");
			n=send(fd,header_1,strlen(header_1),0);
			close(fd);
		}	
				
			
	}
	close(sockfd);
}	

void setTimeout(int milliseconds)
{
    // If milliseconds is less or equal to 0
    // will be simple return from function without throw error
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    // a current time of milliseconds
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;

    // needed count milliseconds of return from this timeout
    int end = milliseconds_since + milliseconds;

    // wait while until needed time comes
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}
	
