#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include<errno.h>
#include<arpa/inet.h>
#define MAX 100 
#define PORT 8080 
#define SA struct sockaddr 
void func(int sockfd,int x) 
{     
       for (;;) { 
       
       File *fi;
       char name[31],loc[32],manuf[64];
       int num;
       fi=fopen("database.txt","w");
       switch(x)
       {
       case 1:
       {
        printf("Enter the name of a device\n");
        //fputs(name,fi);    
        write(sockfd, name, sizeof(name)); //send the name to server
       
        
        printf("Enter the device number\n");
        //fputs(num,fi);
        write(sockfd, num, sizeof(num));
 
        printf("Enter the location of the device\n");
       // fputs(loc,fi);
         write(sockfd, loc, sizeof(loc));

        printf("Enter the manufacturer name\n");
       // fputs(manuf,fi);
         write(sockfd, manuf, sizeof(manuf));
        }
        break;
        
        case 2: //remove a device
        {
        }
         break;
       
        case 3: //edit the details of a device
        {
        }
        break;
         
   

	/*char buff[MAX]; 
	int n; 
	//for (;;) { 
		bzero(buff, sizeof(buff)); 
		//printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n');
 
               //send that buffer to server
		write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s", buff); 
		if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		} 
	} */
} 

int main() 
{ 
	int sockfd, connfd; 
        int x;
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr));           //erases the data by writing zeros to that area containing '\0') to that area.

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //binds to only this particular address
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 
 
 printf("Enter the value of x : \n");
 scanf("%d",&x);
 printf("1.Add a device\n");
 printf("2.Remove a device\n");
 printf("3.Edit the device details\n");
 
        
	// function for chat 
	func(sockfd,x); 

	// close the socket 

	close(sockfd); 
        return 0;
} 

