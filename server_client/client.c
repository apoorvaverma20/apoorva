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
     
       int i=0;
       char arr[100];
       //char value;
       bzero(arr, MAX);
       enum command{Read,writing,add,delete,update}; //0 to 4
       enum attri_id{dev_name,loc,manf_name};
       int8_t dev_id;
       int8_t attr_len;
      // char dev_name,loc,manf_name;
       char name[10];
       enum command k;
       enum attri_id m;
       m=0;
       
       switch(x)
       {
       case 1:      //add a device
       {
        arr[i]=writing;    //command type                //arr[0]
        printf("writing means %d\n",arr[i]);
        while(m<3)     //repeating for all attributes
        {
        i++; 
        // printf("value of i id %d\n",i);

        printf("Enter the deviceID\n");       
        scanf("%hhd",&dev_id);
        arr[i]=dev_id;    //device id                   //arr[1]
        printf("device id is %d\n",arr[i]);
        i++;
       
        arr[i]=m;       //attribute id
        printf("Attribute id is %d\n",arr[i]);
        m++;
        i++;
        //printf("value of i id %d\n",i);

        printf("Enter the value to be inserted\n"); 
        scanf("%s",name);
        printf("Name is %s",name);
        attr_len=strlen(name);
       
        arr[i]=attr_len;                            //arr[2]
        printf("Attribute length is %d\n",arr[i]);
        i++;
        strcpy(arr+i,name);
        //printf("%c %c %c %c",arr[3],arr[4],arr[5],arr[6]);
        //printf("value of i is %d\n",i);
       // int k=strlen(arr);
       // for(int j=0;j<20;j++)
       // printf("Array values of arr[%d] are %d \n",j,arr[j]);

        write(sockfd,arr,sizeof(arr));
        
        }        //end of while loop

        } //end of case
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

