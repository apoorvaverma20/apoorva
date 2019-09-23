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
void func(int sockfd) 
{     
     
       int i=0;
       char arr[100];
       //char value;
       bzero(arr, MAX);
       enum command{Read=0,writing=1,remove=2,edit=3}; //0 to 3
       enum attri_id{dev_name=0,loc=1,manf_name=2};
       int8_t dev_id;
       int x;
       int8_t attr_len;
      // char dev_name,loc,manf_name;
       char name[31];
       enum command k;
       enum attri_id m;
      
         for(;;)
       {
         printf("1.Add a device\n");
         printf("2.Remove a device\n");
         printf("3.Edit the device details\n");
         printf("Enter the value of x : \n");
         scanf("%d",&x);
     
       if(x==1)
       {
        arr[i]=writing;    //command type                //arr[0]
        printf("writing means %d\n",arr[i]);
        i=i+1;
        printf("value of i is %d\n",i);

        printf("Enter the deviceID\n");       
        scanf("%hhd",&dev_id);
        arr[i]=dev_id;    //device id                   //arr[1]
        printf("device id is %d\n",arr[i]);
        i=i+1;
        printf("value of i is %d\n",i);

        arr[i]=dev_name;       //attribute id
        printf("Attribute id arr[%d] is %d\n",i,arr[i]);        //arr[2]
        i=i+1;
        printf("value of i is %d\n",i);

        printf("Enter the value to be inserted\n"); 
        scanf("%s",&name);
        printf("Name is %s\n",name);
      
        attr_len=strlen(name);      
        arr[i]=attr_len;                            //arr[3]
        printf("Attribute length of arr[%d] is %d\n",i,arr[i]);
        i=i+1;
        printf("value of i is %d\n",i);
        
        //i=sizeof(arr);
        i=strlen(arr);
        printf("value of i of array length is %d\n",i);
  
        strcpy(arr+i,name);
        for(int j=0;j<strlen(arr);j++)
        {
        printf("arr[%d] is %c\n",j,arr[j]);
        //i=j;
        }
        m++;
        write(sockfd,arr,sizeof(arr));

        }
        
        } //end of for
        

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
 
        
	// function for chat 
	func(sockfd); 

	// close the socket 

	close(sockfd); 
        return 0;
} 

