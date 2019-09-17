//CORRECTION NEEDED
#include<iostream>
#include<bits/stdc++.h>
using namespace std;

int main()
{
	char temp[100],temp1[100];
	int k=0,tem[100];
	for(char i='A';i<='Z';i++)
	{
	 temp[k]=i;
	 //cout<<temp[k]<<endl;
	 //temp1[k]=temp[k];
	 k++;
	}
	for(int i=0;i<=25;i++)
	{
		tem[i]=int(temp[i]);
	//	cout<<tem[i]<<endl;
	}
         for(int i=0;i<26;i++)
         {
          temp1[i]=hex<<int(temp[i]);
         }

	cout<<"|--Character--|"<<"--ASCII Dec--|"<<"--ASCII Hex--|"<<endl;
	
	for(int i=0;i<26;i++)
	{
		cout<<"   "<<temp[i]<<"     "<<tem[i]<<"     "<<temp1[i]<<"\n";
	}
}
