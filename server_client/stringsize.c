#include<stdio.h>
#include<string.h>

int main()
{
 char arr[100];
 arr[0]=1;
 arr[1]=2;
 arr[2]=5;
 arr[3]=8;
 arr[4]=7;
 
 int i=strlen(arr);
 printf("value of %d\n",i);
 char name[]="apoorva";
 strcpy(arr+i,name);
 printf("arr[0] is %d\n",arr[0]);
 printf("arr[1] is %d\n",arr[1]);
 printf("arr[2] is %d\n",arr[2]);
 printf("arr[3] is %d\n",arr[3]);
 printf("arr[4] is %d\n",arr[4]);
 printf("arr[5] is %c\n",arr[5]);
 printf("arr[6] is %c\n",arr[6]);
 printf("arr[7] is %c\n",arr[7]);
 
// for(int i=0;i<strlen(arr);i++)
 //printf("a[%d] is %c\n",i,arr[i]); 
}
