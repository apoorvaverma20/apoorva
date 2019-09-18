#include <netdb.h> 
#include<stdio.h>
#include<unistd.h>
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#define MAX 100 
#define PORT 8080 
#define SA struct sockaddr 

struct sockaddr_in servaddr, cli; 

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
        int arr[10],z=1;
        File *fi;
        char dev_name[31],loc[32],manf_name[64];
        int dev_num[100]; 
	int n; 
        fi=fopen("database.txt","w");
        if ( fi == NULL )   //if pointer is NULL means no file present
        printf( " file failed to open." ) ; 
        else
        printf("The file is now opened.\n") ; 

	// infinite loop for chat 
	for (;;) { 
                dev_num[i]=z;
	        for(int j=0;j<3;j++)
                {
                 for(int i=0;i<4;i++)
                 {
                  read(sockfd,arr[i],sizeof(arr[i])); 
                 }
                 // add details
                  if(arr[0]==1 && arr[2]==0)       //means writing and arr[2]=device name
                  {
                   printf("Enter the device name");
                   scanf("%s",&dev_name);
                   fputs(dev_name,fi);
                   fputs("\t",fi);
                  }
                  if(arr[0]==1 && arr[2]==1)       //means writing and arr[2]=location
                  {
                   printf("Enter the location");
                   scanf("%s",&loc);
                   fputs(loc,fi);                 //write in file
                   fputs("\t",fi);
                  }

                  if(arr[0]==1 && arr[2]==2)       //means writing and arr[2]=manf name
                  {
                   printf("Enter the manufacture name");
                   scanf("%s",&manf_name);
                   fputs(manf_name,fi);
                   fputs("\t",fi);
                  }
                   fputs("\n",fi);
                   arr[2]=m+1;         //dev name-loc-man_name
                }       
              }
               fclose(fi);
               
               
               
                /* char buff[MAX]; 
		// print buffer which contains the client contents 
		printf("From client: %s\t To client : ", buff); 
		bzero(buff, MAX); 
		n = 0; 
		// copy server message in the buffer 
		while ((buff[n++] = getchar()) != '\n'); 

        
		// and send that buffer to client 
		write(sockfd, buff, sizeof(buff)); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} */
	} 
} 

int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification            
	sockfd = socket(AF_INET, SOCK_STREAM, 0);        
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //binds to all available interfaces
        //INADDR_ANY allow the program to work without knowing the IP address of the machine it was running on.A socket bound to this address receives packets from all interfaces.
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification (server address)
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification.Here server waits for the client to approach the server to make a connection 
	if ((listen(sockfd, 10)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification and ready to transfer data
	connfd = accept(sockfd, (SA*)&cli, &len);  //(SA*)&cli has the address of the client
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

        char ip[INET_ADDRSTRLEN]; 
        inet_ntop(AF_INET, &(cli.sin_addr), ip, INET_ADDRSTRLEN); 
        printf("connection established with IP : %s and PORT : %d\n",  
                                            ip, ntohs(cli.sin_port)); 
  

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 

