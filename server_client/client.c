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
       int i=0;
       char arr[100];
       //char value;
       bzero(arr, MAX);
       enum command{Read,writing,add,delete,update}; //0 to 4
       enum attri_id{dev_name,loc,manf_name};
       char dev_id;
       char attr_len;
      // char dev_name,loc,manf_name;
       char name[100];
       enum command k;
       enum attri_id m;
       switch(x)
       {
       case 1:      //add a device
       {
        while(m<3)     //repeating for all attributes
        {
        arr[i]=writing;    //command type
        i++; 

        printf("Enter the deviceID\n");       
        scanf("%s",&dev_id); 
        arr[i]=dev_id;
        i++;
 
        arr[i]=m;       //attribute id
        m++;
        i++;

        printf("Enter the value to be inserted\n"); 
        scanf("%s",name);
        attr_len=sizeof(name);
        arr[i]=attr_len;
        i++;
  
        strcpy((arr+attr_len),name);
        }        //end of while loop

        write(sockfd,arr,sizeof(arr));
        
        /*for(int j=0;j<3;j++)
        {
         for(int i=0;i<4;i++)
         {
          write(sockfd,arr[i],sizeof(arr[i])); //send the "array" to server
         }
         
         arr[2]=m+1;
        } */   
         
         
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

