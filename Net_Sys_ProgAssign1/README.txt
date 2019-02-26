Implementation of UDP protocol 
The folders Server and Client contain the .c and makefiles of the server and the client respectively. 

Client: 
The file contains 5 options:

1. get :- will command the server to send a specific file as directed by the user for transfer
2. put :- will send the server a specific as directed by the user.
3. delete :- will command the server to delete a specific as directed by the user
4. ls :- will command the server to send a list of files of the local directory (in which the client is present)
5. exit :- will command the server to exit the serve to exit the terminal


The reliability is implemented by sending a number in ascending sequence which will be sent along the packet of bytes and if the not recived will send the file again. Since UDP is not a perfect protocol which does not have 100% efficiency, it may also happen that the acknowledgement is sent but not received so maybe sent the same packet twice. The CRC is not implemented as acknowledgement as it does notincrease the efficiency of the UDP protocol. Could also be replaced by the MD5 hash encryption but did not due to difficulty.

Server:
The server will implement all the five options accordingly as compared to client commands it. The server won't implement unless the client commands it to. So server will respond accordingly.


NOTE: In order to access file from another directory you have to use the "../"  . The ".." will let you move one directory behind. Thus if your makefile is /ABC/CD/makefile. And you have to access a file in /ABC/somefile, use ../somefile.
