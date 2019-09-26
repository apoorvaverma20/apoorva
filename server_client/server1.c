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
              char arr[256],index[256],arr2[256];
              FILE *fi;
              char ch;
              bzero(arr, MAX);
              int t1;              
              read(sockfd,arr,sizeof(arr));
              int z=strlen(arr);
              printf("size of array is %d\n",z);
<<<<<<< HEAD
             // for(int d=0;d<z;d++){
              //arr2[d]=arr[d];
              //printf("i is %x\n",arr2[d]);}
       
       
       if(arr[0]==1)  //add/write
              {  
                          
              int j;
=======
       
             
            if(y==1)
              {
              for(int a=0;a<z;a++)
              printf("i is %x\n",arr[a]);
              int j=0;
>>>>>>> 40d1e30f4e2a83b1b2226bf52734996cb39b6daf
              index[j]=arr[1];
              printf("index[%d] is %d\n",j,arr[1]);
              int k=1;
              int p=4;
              t1=4;
              for(j=1;j<32;j++)
               {
                if(k<=arr[3])
                {
                 index[j]=arr[p];
                 printf("index[%d] is %d\n",j,index[j]);
                 p++;
                 k++;
                }
               else{
                index[j]='0'; 
                printf("index[%d] is %d\n",j,index[j]); }                           
              }              
               int l=1;               
               p++;
               int o=p+1;
               int t2=o;
          
              for(j=32;j<64;j++)
               {
                if(l<=arr[p])
                {
                 index[j]=arr[o];
                  printf("index[%d] is %d\n",j,index[j]);
                 o++;
                 l++;
                }
                else{
                 index[j]='0';
                 printf("index[%d] is %d\n",j,index[j]);}
               }
                o++;
                
                int b=1,q=o+1;
                int t3=q;
                for(j=64;j<256;j++)
                {
                 if(b<=arr[o])
                  {
                   index[j]=arr[q];
                   printf("index[%d] is %d\n",j,index[j]);
                   q++;
                   b++;
                  }
                  else{
                  index[j]='0';
                  printf("index[%d] is %d\n",j,index[j]);}
                }
               int r=strlen(index);
               printf("no of elements in index are %d\n",r);
              
               fi=fopen("database.txt","a");
               if (fi == NULL)                                          //if pointer is NULL means no file present
               printf("file failed to open.") ; 
               else{
               printf("The file is now opened.\n") ; 
               fputs(index,fi);
               }
               fclose(fi);
    } 
 
     if(arr[0]==0)//read
       {  
              fi=fopen("database.txt","r");
              if (fi == NULL)                                          //if pointer is NULL means no file present
              printf("file failed to open.") ; 
              else
              printf("The file is now opened.\n") ; 
              //while((ch = fgetc(fi)) != EOF)
              write(sockfd,index,sizeof(index));
  
       }
           

     if(arr[0]==2)    //delete
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

    if(arr[0]==3) //edit
    {
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

