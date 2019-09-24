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
        char arr[256],index[256];
        FILE *fi;
    
              read(sockfd,arr,sizeof(arr));
              int z=strlen(arr);
              printf("size of array is %d\n",z);
              int i=0,j=0;
              index[j]=arr[i+1];
              int k=1;
              int p=3;
              for(j=1;j<32;j++)
              {
               if(k<=arr[i+2]){
               index[j]=arr[p];
               p++;
               k++;}
               else
               index[j]='0';              
              }
               printf("i,j,k,p are %d %d %d %d\n",i,j,k,p);
               int l=1;
              // index[j]=arr[p]; //attribute length
               printf("Array of p is %d\n",arr[p]);
               int o=p+1;
               for(j=32;j<64;j++)
               {
                 if(l<=arr[p]){
                 index[j]=arr[o];
                 o++;
                 l++;}
                 else
                 index[j]='0';
               }
                printf("l,o,p,j are %d %d %d %d\n",l,o,p,j);
                
                int b=1,q=o+1;
                for(j=54;j<256;j++)
                {
                 if(b<=arr[o])
                  {
                   index[j]=arr[q];
                   q++;
                   b++;
                   }
                  else
                  index[j]='0';
                }
                 
                
       
            
               fi=fopen("database.txt","a");
               if ( fi == NULL )   //if pointer is NULL means no file present
               printf( "file failed to open." ) ; 
               else{
               printf("The file is now opened.\n") ; 
               fputs(index,fi);
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
        //printf("connection established with IP : %s and PORT : %d\n",  
                                         //   ip, ntohs(cli.sin_port)); 
         
	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 

