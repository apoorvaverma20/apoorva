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
       char name[32],location[32];
       //enum command k;
       enum attri_id m;
       m=0;
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
   
        
        /*arr[i]=dev_name;       //attribute id
        printf("Attribute id arr[%d] is %d\n",i,arr[i]);        //arr[2]
        i=i+1;
        printf("value of i is %d\n",i);*/
        while(m<3)
        {
        if(m==0)
       { printf("Enter the device name to be inserted\n"); 
        scanf("%s",&name);
        printf("Name is %s\n",name);}
       if(m==1)
       {printf("Enter the device location to be inserted\n"); 
        scanf("%s",&name);
        printf("Name is %s\n",name);}
       if(m==2)
       {printf("Enter the manufacturer name to be inserted\n"); 
        scanf("%s",&name);
        printf("Name is %s\n",name);}
      
        attr_len=strlen(name);      
        arr[i]=attr_len;                            //arr[3]
        printf("Attribute length of arr[%d] is %d\n",i,arr[i]);
       // i=i+1;
        printf("value of i is %d\n",i);
        i=strlen(arr);             //arr[4]
       // printf("value of array length is %d\n",i);
        
      strcpy(arr+i,name);
      i=strlen(arr);
      printf("value of array length is %d\n",i);
     /* printf("arr[0] is %d\n",arr[0]);
      printf("arr[1] is %d\n",arr[1]);
      printf("arr[2] is %d\n",arr[2]);
      printf("arr[3] is %c\n",arr[3]);
      printf("arr[4] is %c\n",arr[4]);
      printf("arr[5] is %c\n",arr[5]);
      printf("arr[6] is %c\n",arr[6]);
      printf("arr[7] is %c\n",arr[7]);*/

      m++;
      } //end of while loop
    }//end of if
  write(sockfd,arr,sizeof(arr));
         
	
} 

int main() 
{ 
	int sockfd, connfd; 
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

