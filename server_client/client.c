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
       for (;;)
      { 
       int arr[10];
       char value[100];
       bzero(arr, MAX);
       enum command{Read,writing,add,delete,update}; //0 to 4
       enum attri_id{dev_name,loc,manf_name};
       int dev_id;
       int attr_len;
       enum command k;
       enum attri_id m;
       switch(x)
       {
       case 1:      //add a device
       {
       // printf("Enter the command type\n");
        //scanf("%d",&k);
        //k=writing;  //store 1 here
        arr[0]=x;    //command type
        
        printf("Enter the deviceID\n");
        scanf("%d",&dev_id); 
        arr[1]=dev_id;
 
        printf("Enter the attribute ID\n");
        //m=dev_name;   //store 0
        arr[2]=m;
 
        //printf("Enter the attribute value length\n");
        attr_len=sizeof(arr[2]);
        arr[3]=attr_len; 

         printf("Enter the value\n");
        scanf("%d",value); 
        arr[4]=value;
        
        /*for(int j=0;j<3;j++)
        {
         for(int i=0;i<4;i++)
         {
          write(sockfd,arr[i],sizeof(arr[i])); //send the "array" to server
         }
         
         arr[2]=m+1;
        } */   
         
         write(sockfd,arr,sizeof(arr));
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
        
        }
        

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
		} */
	} 
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
 
         printf("1.Add a device\n");
         printf("2.Remove a device\n");
         printf("3.Edit the device details\n");
         printf("Enter the value of x : \n");
         scanf("%d",&x);
        
	// function for chat 
	func(sockfd,x); 

	// close the socket 

	close(sockfd); 
        return 0;
} 

