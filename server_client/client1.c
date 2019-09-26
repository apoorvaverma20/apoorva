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
     
       int i;
       char arr[256],index[256];
       bzero(arr, MAX);
       enum command{reading=0,writing=1,deleting=2,edit=3};                 //0 to 3
       enum attri_id{dev_name=1,loc=2,manf_name=3};
       int8_t dev_id;
       int y;
       int8_t attr_len;
       char name[32],location[32],manufc[32];
       //enum command k;
       enum attri_id m;
       m=0;
       for(;;){
         printf("0.Read a device details\n");
         printf("1.Add a device\n");
         printf("2.Remove a device\n");
         printf("3.Edit the device details\n");
         printf("Enter the value of y : \n");
<<<<<<< HEAD
         scanf("%d",&y);      
      
    if(y==0)         
    {
         i=0;
         arr[i]=reading;       //arr[0]
         printf("read means %d\n",arr[i]);
         i=i+1;
         printf("value of i is %d\n",i);
       
         printf("Enter the deviceID\n");    
         scanf("%hhd",&dev_id);
         arr[i]=dev_id;                    //arr[1]  
         printf("device id is %d\n",arr[i]);

         read(sockfd,index,sizeof(index));
         for(int k=0;k<256;k++)
         {
          printf("%c\n", index[k]);
         }
     }        //end of if 
                     
   if(y==1)
   {
         i=0;
=======

         scanf("%d",&y);         

         scanf("%d",&y);
         write(sockfd, y , sizeof(y)); 

         printf("value of y is %d\n",y);
     
        if(y==1)
        {
         write(sockfd, y , sizeof(y));
>>>>>>> 40d1e30f4e2a83b1b2226bf52734996cb39b6daf
         arr[i]=writing;    //command type                               //arr[0]
         printf("writing means %d\n",arr[i]);
         i=i+1;
         printf("value of i is %d\n",i);
        
         printf("Enter the deviceID\n");      //start from 0 
         scanf("%hhd",&dev_id);
         arr[i]=dev_id;    //device id                                   //arr[1]
         printf("device id is %d\n",arr[i]);
         i=i+1;
         arr[i]=dev_name;
         printf("value of i is %d\n",i);
         
         while(m<3)
         {
          if(m==0)
          { 
           printf("Enter the device name to be inserted\n"); 
           scanf("%s",&name);
           printf("Name is %s\n",name);
           i=i+1;
          }
          if(m==1)
          {
          arr[i]=loc;
          printf("Enter the device location to be inserted\n"); 
          scanf("%s",&name);
          printf("Name is %s\n",name);
          i=i+1;
          }
         if(m==2)
         {
          arr[i]=manf_name;
          printf("Enter the manufacturer name to be inserted\n"); 
          scanf("%s",&name);
          printf("Name is %s\n",name);
          i=i+1;
         }
               
         attr_len=strlen(name);      
         arr[i]=attr_len;                                                 //arr[3]
         printf("Attribute length of arr[%d] is %d\n",i,arr[i]);
         // i=i+1;
         printf("value of i is %d\n",i);
         i=strlen(arr);                                                   //arr[4]
        
         strcpy(arr+i,name);
         i=strlen(arr);
         printf("value of array length is %d\n",i);
         m++;
       }                                                                //end of while loop
   }                                                                    //end of if
       
   if(y==2)
   { 
         i=0;
         arr[i]=deleting;    //arr[0]
         printf("deleting means %d\n",arr[i]);
         i=i+1;
         printf("value of i is %d\n",i);
      
         printf("Enter the deviceID\n");       
         scanf("%hhd",&dev_id);
         arr[i]=dev_id;    //device id    //arr[1]                                
         printf("device id is %d\n",arr[i]);
         
   }                //end of if
   if(y==3)           
   { 
         i=0;
         arr[i]=edit; //arr[0]
         printf("edit means %d\n",arr[i]);
         i=i+1;
         printf("value of i is %d\n",i);
         
         printf("Enter the deviceID\n");       
         scanf("%hhd",&dev_id);
         arr[i]=dev_id;    //device id    //arr[1]                                
         printf("device id is %d\n",arr[i]);
         i=i+1;

         printf("enter the attribute id to edit\n");
         scanf("%d",&m);
         printf("attribute id is %d\n",m);
         arr[i]=m;
         printf("attribute id is %d\n",arr[i]);      
   }                  //end of if     
     write(sockfd,arr,sizeof(arr));    
   
   }       
             //end of infinite for
 }                   //end of function

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

