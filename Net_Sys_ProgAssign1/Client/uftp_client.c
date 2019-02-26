/* 
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024

int option;
char input_user[5];

    
/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

void command()
{
	printf("Get user command:\n");
	printf("\n1. get\n2. put\n3. delete\n4. ls\n5. exit\n");
	scanf("%s",input_user);
	printf("%s\n",input_user);
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
	
	

}

 

int main(int argc, char **argv) {
	int sockfd, portno, n;
    	int serverlen;
    	struct sockaddr_in serveraddr;
	//struct sockaddr_in serveraddr; /* server's addr */
  	struct sockaddr_in clientaddr;
    	struct hostent *server;
    	char *hostname;
    	char buf[BUFSIZE];
	char filename[100];
	char delfile[100];
	int loop,i,j,count=0;
	int ack;
 	char length[10];
	char list[65000];
 	int length_file,residue;
	int nlists;
	FILE *fp;
    	/* check command line arguments */
    	if (argc != 3) {
       		fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       		exit(0);
    	}
    	hostname = argv[1];
    	portno = atoi(argv[2]);

    	/* socket: create the socket */
    	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    	if (sockfd < 0) 
        	error("ERROR opening socket");

    	/* gethostbyname: get the server's DNS entry */
    	server = gethostbyname(hostname);
    	if (server == NULL) {
        	fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        	exit(0);
    	}

    	/* build the server's Internet address */
    	
	bzero((char *) &serveraddr, sizeof(serveraddr));
    	serveraddr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr, 
	 	 (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    	serveraddr.sin_port = htons(portno);

    	/* ---------------------------
	 * get a message from the user 
	 * ---------------------------*/
    	
	while(1)
	{
	bzero(buf, 100);
com:
	command();
    	strcpy(buf,input_user);
    	printf("%d",option);
    
    	/*now depending on the option do different commands*/
 
    	/* send the message to the server */

    	serverlen = sizeof(serveraddr);
    	n = sendto(sockfd, buf, 100, 0, &serveraddr, serverlen);
    	if (n < 0) 
      		error("ERROR in sendto");
    
   	/* * print the server's reply */
 	/*   n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
    	if (n < 0) 
      	error("ERROR in recvfrom");
    	printf("Echo from server: %s", buf);
    
   	*/

 	switch(option)
    	{
		/*-----------------------------------
		 * Client commands the server to send 
		 * the required file
		 * ----------------------------------*/
		case 1:
			//Clearing the buffer to avoid data corruption
			bzero(buf,100);
			bzero(length,10);
			bzero(filename,100);
			//Asking the filename
			printf("Enter filename\n");
			scanf("%s",filename);
			/*Sending the file name and getting the size from
			 * the server*/
			n=sendto(sockfd,filename,100,0,&serveraddr,serverlen);
			if(n<0)
				error("ERROR in sendto");
			n = recvfrom(sockfd, length, 10, 0,
                                (struct sockaddr *) &serveraddr, &serverlen);

                        if (n < 0)
                                error("ERROR in recvfrom");
                        length_file=atoi(length);
			printf("%d",length_file);
			
                        bzero(buf,100);

                        loop=length_file/100;
                        residue=length_file%100;
                        fp=fopen("foo_test","w+");
			/* File reception*/
                        for(i=0;i<loop;i++)
                        {

                                bzero(length,10);
                                bzero(buf,100);
ackgive:
                                n = recvfrom(sockfd, buf, 100, 0,
                                        (struct sockaddr *) &serveraddr, &serverlen);

                                if (n < 0)
					error("ERROR in recvfrom");
				n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&serveraddr,&serverlen);
				n=sendto(sockfd,length, 10,0,&serveraddr,serverlen);
				ack=atoi(length);
				if(ack==count)
                                {
                                        fwrite(buf,1,100,fp);
                                        printf("ack=%ld\n",ack);
                                        printf("count=%d\n",count);
                                        count++;
                                }
                                else
                                {
                                        printf("\nFile not recived\n");
                                        goto ackgive;
                                }


                        }
              
                  	//Running for the last bytes of the file
			if(residue!=0)
                        {
recack:
				bzero(buf,100);
                                bzero(length,10);
				n=sendto(sockfd,length,10,0,&serveraddr,serverlen);
				if(n<0)
					error("ERROR in sendto");
				n = recvfrom(sockfd, buf, residue, 0,(struct sockaddr *) &serveraddr, &serverlen);
                                                              
				if (n < 0){
                                        error("ERROR in recvfrom");
                                }
			
			//	clientlen=sizeof(clientaddr);

                                fwrite(buf,1,residue,fp);
				printf("Acknowledgement=%s\n",length);
				fclose(fp);
				break;
                        
			}
                        

		/*------------------------------------
		 * The client gives the file to the server
		 * -------------------------------------*/
		
		case 2:
			bzero(buf, 100);
    			bzero(length,10);
			bzero(filename,100);

			/*Filename to transmit the file*/
			
			printf("Enter filename\n");
			scanf("%s",filename);
    			fp=fopen(filename,"r");
			if(fp==NULL)
			{
				printf("File does not exist\n");
				exit(0);
			}

			/*-----------------------------
			 * The file size is determined and
			 * transmitted via the socket
			 * -----------------------------*/
    			fseek(fp,0,SEEK_END);
    			length_file=ftell(fp);
    			printf("%ld\n",length_file);
    			sprintf(length,"%ld",length_file);
    			serverlen = sizeof(serveraddr);
    			fseek(fp,0,SEEK_SET);
    			n=sendto(sockfd,length,10,0,&serveraddr,serverlen);
    			if(n<0)
	    			error("Error in sendto");
    			loop=length_file/100;
    			residue=length_file%100;
    			for(i=0;i<loop;i++)
    			{
       
    				bzero(buf,100); 
    				bzero(length,10);
   				// sprintf(length,"%d",count);
    				fread(buf,1,100,fp);	
    				/* send the message to the server */
				//    serverlen = sizeof(serveraddr);
ackrec:
    				sprintf(length,"%ld",count);
    				n = sendto(sockfd, buf, 100, 0, &serveraddr, serverlen);
    				if (n < 0) 
      					error("ERROR in sendto1");
    				n = sendto(sockfd,length,10,0,&serveraddr,serverlen);
    				if(n<0)
	    				error("ERROR in sendto2");

    
    				bzero(length,10);
    				n = recvfrom(sockfd,length,10,0,(struct sockaddr *)&serveraddr,&serverlen);
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
	    			/*	n=sendto(sockfd,"send",4,0,&serveraddr,serverlen);
					if(n<0)
						error("ERROR in sendto3");
				*/	goto ackrec;
    				}		

    
   			}
    			if(residue!=0)
    			{
cliack:
				bzero(buf,100);
	    			fread(buf,1,residue,fp);
	    			n=sendto(sockfd,buf,residue,0,&serveraddr,serverlen);
	    			if(n<0)
		    			error("Residue send error");
				bzero(length,10);
				n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&serveraddr,&serverlen);
				if(n<0)
					error("ERROR in recvfrom");
    				n=sendto(sockfd,length,10,0,&serveraddr,serverlen);
				if(n<0)
					error("ERROR in sendto");
				if(strcmp(length,"ack"))
				{
					printf("not recived\n");
					goto cliack;
				}
			
			fclose(fp);
			}
			break;


		/*-------------------------------------
		 * Command the server to delete the given or exit
		 * or exit if the file does not exist
		 * ------------------------------------*/
		
		
		case 3:
filedelete:
			bzero(delfile,100);
			printf("Enter the path to delete the file\n");
			scanf("%s",delfile);
			n=sendto(sockfd,delfile,100,0,&serveraddr,serverlen);
			if(n<0)
				error("ERROR in sendto1");
			bzero(delfile,100);
			n=recvfrom(sockfd,delfile,3,0,(struct sockaddr *)&serveraddr,&serverlen);
			if(n<0)
				error("ERROR in recvfrom");
			n=sendto(sockfd,delfile,3,0,&serveraddr,serverlen);
			if(n<0)
				error("ERROR in sendto");
			if(strcmp(delfile,"ack")!=0)
			{
				printf("No acknowledgement");
				goto filedelete;
			}
			break;			
		
		/*------------------------------------
		 * Command the server to send the list
		 * of files to the client and save in 
		 * a file
		 * -----------------------------------*/
		
		case 4:
listfile:
			printf("Requesting entry of files\n");
			bzero(list,100);
			FILE *fp2=fopen("lists1","a+");
			n=sendto(sockfd,length,10,0,&serveraddr,serverlen);
                        if(n<0)
				error("ERROR in sendto");
			bzero(length,10);
			n=recvfrom(sockfd,length,10,0,(struct sockaddr *)&serveraddr,&serverlen);

                        if(n<0)
                                error("ERROR in recvfrom");

			
			length_file=atoi(length);
			for(i=0;i<length_file;i++)
			{
				n=recvfrom(sockfd,list,1,0,(struct sockaddr *)&serveraddr,&serverlen);
                        	if(n<0)
                                	error("ERROR in recvfrom");
				fwrite(list,1,1,fp2);

				n=sendto(sockfd,"ack",strlen("ack"),0,&serveraddr,serverlen);
				if(n<0)
					error("ERROR in sendto");
				bzero(length,10);
				n=recvfrom(sockfd,length,3,0,(struct sockaddr *)&serveraddr,&serverlen);
                                if(n<0)
                                        error("ERROR in recvfrom");
				if(strcmp(length,"ack")!=0)
				{
					printf("list not available");
					goto listfile;
				}
           
		
		
			}
			fclose(fp2);
			break;
		
		/*--------------------
		 * Exit gracefully
		 * ------------------*/
		case 5:
			printf("\nThank You\n");
			exit(0);
			break;

		default:
			bzero(buf,100);
			n=sendto(sockfd,length,10,0,&serveraddr,serverlen);
			if(n<0)
				error("ERROR in sendto");
			n=recvfrom(sockfd,buf,10,0,(struct sockaddr *)&serveraddr,&serverlen);
			if(n<0)
				error("ERROR in recvfrom");
			goto com;
			break;
    	}
	
	}
    	close(sockfd);
}
