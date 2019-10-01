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
              char arr[2560],index[2560];
              FILE *fi;
              char ch;
              bzero(arr, MAX);            
              read(sockfd,arr,sizeof(arr));
              int z=strlen(arr);
              printf("size of array is %d\n",z);
              for(int j=0;j<z;j++)
              printf("value in array are %d\n",arr[j]);
               fi=fopen("database.txt","a+");
               if (fi == NULL)                                          //if pointer is NULL means no file present
               printf("file failed to open.") ; 
               else
               printf("The file is now opened.\n") ; 
    for(;;)
    { 
       if(arr[0]==1)  //add/write
       {                                        
              int j=256*(arr[1]-1);
              int m=j+32;
              int n=j+64;
              int r=j+256;
              index[j]=arr[1];
              fputc(index[j],fi);
              printf("index[%d] is %d\n",j,arr[1]);
              int k=1;
              int p=j+4;
              int e=j+3;  
              for(j=j+1;j<m;j++)
               {
                if(k<=arr[e])
                {
                 index[j]=arr[p];
                 fputc(index[j],fi);
                 printf("index[%d] is %d\n",j,index[j]);
                 p++;
                 k++;
                }
               else{
                index[j]='0'; 
                fputc(index[j],fi);
                printf("index[%d] is %d\n",j,index[j]); }                           
              }     //for          
                         
               int l=1;               
               p++;
               int o=p+1;          
              for(j=m;j<n;j++)
               {
                if(l<=arr[p])
                {
                 index[j]=arr[o];
                 fputc(index[j],fi);
                  printf("index[%d] is %d\n",j,index[j]);
                 o++;
                 l++;
                }
                else{
                 index[j]='0';
                 fputc(index[j],fi);
                 printf("index[%d] is %d\n",j,index[j]);}
               }  //for
                o++;                
                int b=1,q=o+1;
                for(j=n;j<r;j++)
                {
                 if(b<=arr[o])
                  {
                   index[j]=arr[q];
                   fputc(index[j],fi);
                   printf("index[%d] is %d\n",j,index[j]);
                   q++;
                   b++;
                  }
                  else{
                  index[j]='0';
                  fputc(index[j],fi);
                  printf("index[%d] is %d\n",j,index[j]);
                  }
               }  //for
           // int s=strlen(index);
            //printf("no of elements in index are %d\n",s);
                             
       } //if
               fclose(fi);
    
 
     if(arr[0]==0)//read
       {  
              fi=fopen("database.txt","r");
              if (fi == NULL)                                          //if pointer is NULL means no file present
              printf("file failed to open.") ; 
              else
              printf("The file is now opened.\n") ; 
              ch = fgetc(fi);
              while (ch != EOF)
              {
              // printf ("%c", ch);
               write(sockfd,ch,sizeof(ch));
               ch = fgetc(fi);
              }
       }
           

     if(arr[0]==2)    //delete
      {
           for(int i=256*(arr[1]-1);i<256*arr[1];i++)
            {
             index[i]='\0';
            }
               
           fi=fopen("database.txt","w+");
           if (fi == NULL)   //if pointer is NULL means no file present
           printf("file failed to open.") ; 
           else{
           printf("The file is now opened.\n") ; 
           fputc (index,fi);
           }
           fclose(fi);
        }

    if(arr[0]==3) //edit
    {
     int i,j=4,k;
     fi=fopen("database.txt","w+");
     if (fi == NULL)   //if pointer is NULL means no file present
       printf("file failed to open."); 
     else
       printf("The file is now opened.\n");
  
      if(arr[2]==1)
      {k=1;
       for(i=1;i<32 && k<arr[3];i++)
      {
       index[i]=arr[j];
       i++,j++,k++;
       fputc(index,fi);}
      }
     if(arr[2]==2)
     {k=1;
      for(i=32;i<64 && k<arr[3];i++)
      {
       index[i]=arr[j];
       i++,j++,k++;
       fputc(index,fi);}
      }
      if(arr[2]==3)
      {k=1;
      for(i=64;i<256 && k<arr[3];i++)
      {
       index[i]=arr[j];
       i++,j++,k++;
       fputc(index,fi);}
      }
       
    }

          fclose(fi);
    if(!arr[0])
    {
      printf("Entered wrong choice\n");
      printf("Server Exit...\n"); 
      break; 
    }
 } //infinite for loop
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

