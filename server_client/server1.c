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

      void func(int sockfd)                              // Function designed for chat between client and server.
      { 
              char arr[256],index[256];
              FILE *fi;
              bzero(arr, MAX);
              int y;
              read(sockfd, y , sizeof(y));
              
              read(sockfd,arr,sizeof(arr));
              int z=strlen(arr);
              printf("size of array is %d\n",z);
              printf("enter the value of y \n");
              scanf("%d",&y);
             
            if(y==1)
              {
              for(int a=0;a<z;a++)
              printf("i is %x\n",arr[a]);
              int j=0;
              index[j]=arr[1];
              int k=1;
              int p=4;
              for(j=1;j<32;j++)
               {
                if(k<=arr[3])
                {
                 index[j]=arr[p];
                 p++;
                 k++;
                }
               else
                index[j]='0';              
              }
               printf("j,k,p are %d %d %d\n",j,k,p);
               int l=1;
               // index[j]=arr[p]; //attribute length
               printf("Array of p is %d\n",arr[p]);
               p++;
               int o=p+1;
               printf("value of p and in array is %d %d\n",p,arr[p]);
              for(j=32;j<64;j++)
               {
                if(l<=arr[p])
                {
                 index[j]=arr[o];
                 o++;
                 l++;
                }
                else
                 index[j]='0';
               }
                o++;
                printf("l,o,p,j and array of o is %d %d %d %d %d\n",l,o,p,j,arr[o]);
                
                int b=1,q=o+1;
                //printf("array of q is %d\n",arr[q]);
                for(j=64;j<256;j++)
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
               int r=strlen(index);
               printf("no of elements in index are %d\n",r);
                 
               fi=fopen("database.txt","a");
               if (fi == NULL)   //if pointer is NULL means no file present
               printf("file failed to open.") ; 
               else{
               printf("The file is now opened.\n") ; 
               fputs(index,fi);
               }
               fclose(fi);
            } //end of if

         if(y==2)
          {
            for(int i=0;i<256;i++)
            {
             index[i]='0';
            }
               
           fi=fopen("database.txt","w");
           if (fi == NULL)   //if pointer is NULL means no file present
           printf("file failed to open.") ; 
           else{
           printf("The file is now opened.\n") ; 
           fputs(index,fi);
           }
           fclose(fi);
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
        //printf("connection established with IP : %s and PORT : %d\n",  
                                         //   ip, ntohs(cli.sin_port)); 
         
	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 

