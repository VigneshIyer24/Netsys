/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
 */

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
#define BUFSIZE 1024
int option;
char input_user[5];
/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}
void command()
{
	
	if(!(strcmp(input_user,"get")))
	{
		option=1;
	}
	else if(!(strcmp(input_user,"put")))
	{
		option=2;
	}
	else if(!(strcmp(input_user,"delete")))
	{
		option=3;
	}
	else if(!(strcmp(input_user,"ls")))
	{
		option=4;
	}
	else if(!(strcmp(input_user,"exit")))
	{
		option=5;
	}
	else
	{
		option=0;
	}
}

int main(int argc, char **argv) {

	int sockfd; /* socket */
	int portno; /* port to listen on */
	int clientlen; /* byte size of client's address */
	struct sockaddr_in serveraddr; /* server's addr */
	struct sockaddr_in clientaddr; /* client addr */
	struct hostent *hostp; /* client host info */
  	char buf[BUFSIZE]; /* message buf */
	char *hostaddrp; /* dotted decimal host addr string */
	char filename[100];
	char delfile[100];
	int optval; /* flag value for setsockopt */
	int n; /* message byte size */
	int count=0;
    	int loop, i,ack;
    	int serverlen;
    	int length_file,residue;
    	char length[10];
	char list[65000];
	int nlists;
    
  	FILE *fp1;
	FILE *fp3;
    	DIR *directory;
	struct dirent *dir;
//	directory = opendir(".");
/*  	if (directory) 
	{
    		while ((dir = readdir(directory)) != NULL) 
		{
      			//strcpy(list,dir->d_name);
			
		}
    	}
    	closedir(directory);
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
  	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

  	/* 
   	* main loop: wait for a datagram, then echo it
   	*/
  	serverlen = sizeof(clientaddr);
  

    	/*
     	* recvfrom: receive a UDP datagram from a client
     	*/
    	while(1)
	{
	bzero(buf, 100);
    	n = recvfrom(sockfd, buf, 100, 0,
		 (struct sockaddr *) &clientaddr, &clientlen);
    	if (n < 0)
      		error("ERROR in recvfrom");
   	// printf("Hello2\n");
	
	
    	strcpy(input_user,buf);
	clientlen=sizeof(clientaddr); 
    	printf("%s\n",input_user);
	command();
      	switch(option)
    	{
		/*------------------------------------------
		 * The client commands the server for a file
		 * -----------------------------------------*/
		case 1:
			bzero(buf, 100);
                        bzero(length,10);
                        bzero(filename,100);
                        
			printf("Get filename\n");
                  	n=recvfrom(sockfd,filename,100,0,(struct sockaddr *)&clientaddr,&clientlen);
			if(n<0)
				error("ERROR in sendto");
                        
			fp1=fopen(filename,"r");
                        if(fp1==NULL)
                        {
                                printf("File does not exist\n");
                                exit(0);
                        }
                        /*-----------------------------------------
			 * Determine the size of the given file and
			 * send to the client for creating a new file
			 * ------------------------------------------*/
			fseek(fp1,0,SEEK_END);
                        length_file=ftell(fp1);
                        printf("%ld\n",length_file);
                        sprintf(length,"%ld",length_file);
                        clientlen = sizeof(clientaddr);
                        fseek(fp1,0,SEEK_SET);
                        n=sendto(sockfd,length,10,0,&clientaddr,clientlen);
                        if(n<0)
                                error("Error in sendto");
			loop=length_file/100;
                        residue=length_file%100;
                        for(i=0;i<loop;i++)
                        {

                                bzero(buf,100);
                                bzero(length,10);
                                fread(buf,1,100,fp1);
ackrec:
                                sprintf(length,"%ld",count);
                                n = sendto(sockfd, buf, 100, 0, &clientaddr, clientlen);
                                if (n < 0)
                                        error("ERROR in sendto");
                                n = sendto(sockfd,length,10,0,&clientaddr,clientlen);
				if(n<0)
                                        error("ERROR in sendto");
                                bzero(length,10);
				n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&clientaddr,&clientlen);
				if(n<0)
					error("ERROR in recvfrom");
				ack=atoi(length);
                                printf("Packet=%ld\n",ack);
                                if(ack==count)
                                {
                                        printf("count = %ld\n",count);
                                        count++;
                                }
                                else
                                {
                                        printf("\nPacket not recived\n");
                                        goto ackrec;
                                }
				

                        }
                        if(residue!=0)
                        {
cliack:
				bzero(buf,100);
				bzero(length,10);
                                fread(buf,1,residue,fp1);
				n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&clientaddr,&clientlen);
				if(n<0)
					error("ERROR in recvfrom");
                                n=sendto(sockfd,buf,residue,0,&clientaddr,clientlen);
			 	if(n<0)
                                        error("Residue send error");
				fclose(fp1);
				break;
			
			}
                        

		/*------------------------------------------
		 * The server recives a file from the client
		 * -----------------------------------------*/
		case 2:
			bzero(buf, 100);
   			//Recieve the file length from the client
    			n = recvfrom(sockfd, buf, 100, 0,
                 		(struct sockaddr *) &clientaddr, &clientlen);
  
    			if (n < 0)
      				error("ERROR in recvfrom");
    			length_file=atoi(buf);
    			bzero(buf,100);
			
			/*The number of loops for a limited buffer length and 
    			 *residue for the remaining bytes*/ 
			loop=length_file/100;
    			residue=length_file%100;
    			fp1=fopen("foo12","w+");
    			for(i=0;i<loop;i++)
    			{
	 
	    			bzero(length,10);
	    			bzero(buf,100);
				printf("Hello\n");
ackgive:
	    			n = recvfrom(sockfd, buf, 100, 0,
                 			(struct sockaddr *) &clientaddr, &clientlen);
    	   
	    			if (n < 0)
    	    			{	
		    			error("ERROR in recvfrom");
            			}
	    			n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&clientaddr,&clientlen);
	    			if (n < 0)
                                {
                                        error("ERROR in recvfrom");
                                }

				n=sendto(sockfd,length,10,0,&clientaddr,clientlen);
	    			if (n < 0)
                                {
                                        error("ERROR in sendto");
                                }

				ack=atoi(length);
	    			printf("%d\n",ack);
				if(ack==count)
				{
					fwrite(buf,1,100,fp1);
					printf("ack=%ld\n",ack);
    	     				printf("count=%d\n",count);
	    				count++;	
    	    			}
    	    			else
	    			{
		    			printf("\nFile not recived\n");
/*		    			n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&clientaddr,&clientlen);
					if (n < 0)
                                {
                                        error("ERROR in recvfrom");
                                }
*/
					goto ackgive;
	    			}
	
	    		
    			}
    			if(residue!=0)
    			{
recack:
				bzero(buf,100);
            			n = recvfrom(sockfd, buf, residue, 0,
                 			(struct sockaddr *) &clientaddr, &clientlen);
	
				if (n < 0){
      					error("ERROR in recvfrom");
				}
				bzero(length,10);
				n=sendto(sockfd,"ack",3,0,&clientaddr,clientlen);
				if(n<0)
					error("ERROR in sendto");
				n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&clientaddr,&clientlen);
				if(n<0)
					error("ERROR in sendto");
				if(strcmp(length,"ack"))
				{
					printf("not recived\n");
					goto recack;
				}
				fwrite(buf,1,residue,fp1);
				fclose(fp1);
				break;

    			}
			break;

		/*--------------------------------------------
		 * The server deletes the given file by the client
		 * ---------------------------------------------*/
		
		case 3:
delfile:
			bzero(delfile,100);
			n=recvfrom(sockfd,delfile,100,0,(struct sockaddr *)&clientaddr,&clientlen);
			if(n<0)
				error("ERROR in recvfrom");
			printf("The file to be deleted is %s\n",delfile);
			n=remove(delfile);
			if(n!=0)
			{
				printf("File does not exist\n");
				break;
			}
			n=sendto(sockfd,"ack",strlen("ack"),0,&clientaddr,clientlen);
			if(n<0)
				error("ERROR in sendto");
			bzero(delfile,100);
			n=recvfrom(sockfd,delfile,3,0,(struct sockaddr *)&clientaddr,&clientlen);
			if(n<0)
				error("ERROR in recvfrom");
			if(strcmp(delfile,"ack")!=0)
			{
				printf("No ackknowledgement\n");
				goto delfile;
			}
			
					
			printf("Success\n");
			break;
		
		/*-----------------------------------------
		 * The server sends a list of files to server
		 * of the local directory
		 * ----------------------------------------*/

		case 4:
			bzero(list,65000);
			bzero(length,10);
			nlists=sizeof(list)/sizeof(list[0]);
			sprintf(length,"%d",nlists);
			n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&clientaddr,&clientlen);
			if(n<0)
				error("ERROR in recvfrom");
			bzero(list,65000);
listfile:
			clientlen=sizeof(clientaddr);
			directory = opendir(".");
			fp3=fopen("lists","a+");
			if (directory)
        		{
                		
				while ((dir = readdir(directory)) != NULL)
                		{
				
				
					strcpy(list,dir->d_name);
					fwrite(list,1,strlen(list),fp3);
				}	fclose(fp3);
			}
        		closedir(directory);
			FILE *fp4=fopen("lists","r");
			fseek(fp4,0,SEEK_END);
			length_file=ftell(fp4);
			fseek(fp4,0,SEEK_SET);
			bzero(length,10);
			sprintf(length,"%d",length_file);
			printf("%d",length_file);
			n=sendto(sockfd,length,strlen(length),0,&clientaddr,clientlen);
                        if(n<0)
                                error("ERROR in sendto");
			for(i=0;i<length_file;i++)
			{
			
				fread(list,1,1,fp4);
				n=sendto(sockfd,list,1,0,&clientaddr,clientlen);
				if(n<0)
					error("ERROR in sendto");
				bzero(length,10);
				n=recvfrom(sockfd,length,3,0,(struct sockaddr *)&clientaddr,&clientlen);
				if(n<0)
                               		error("ERROR in recvfrom");
				n=sendto(sockfd,length,3,0,&clientaddr,clientlen);
                                if(n<0)
                                        error("ERROR in sendto");
				if(strcmp(length,"ack")!=0)
				{
					printf("List not sent\n");
					goto listfile;
				}
				if(i==length_file)
				{
					fclose(fp4);
				
				}
				
			}
			break;
		
		/*---------------------------------------
		 *  The server exits gracefully as commmanded
		 *  by the client
		 *  -------------------------------------*/

		case 5:
			printf("\nThank You\n");
			exit(0);
			break;	
  		
		case 0: 
			n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&clientaddr,&clientlen);
			if(n<0)
				error("ERROR in recvfrom");
			n=sendto(sockfd,buf,10,0,&clientaddr,clientlen);
			 if(n<0)
				 error("ERROR in sendto");
			 break;

	}

    /* 
     * gethostbyaddr: determine who sent the datagram
     */
    	hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
				  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    	if (hostp == NULL)
      		error("ERROR on gethostbyaddr");
    	hostaddrp = inet_ntoa(clientaddr.sin_addr);
    	if (hostaddrp == NULL)
      		error("ERROR on inet_ntoa\n");
    /*	printf("server received datagram from %s (%s)\n", 
	   hostp->h_name, hostaddrp);
    	printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);
  */
   
	}
      	close(sockfd);
}
