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
 int k=strlen(name);
 printf("length of name is %d\n",k);
 strcpy(arr+i,"apoorva");
 for(int i=0;i<strlen(arr);i++)
 printf("a[%d] is %c\n",i,arr[i]);
}
