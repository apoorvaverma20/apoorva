#include<bits/stdc++.h>
using namespace std;

int del(string s1)
{
	char temp[100];
	int k;
	int len=s1.length();
	for(int i=0;i<s1.length();i++)
	{
		temp[i]=s1.at(i);
	}
	for(int j=0;j<len;j++)
    {
    	if(temp[j]=='a'||temp[j]=='e'||temp[j]=='i'||temp[j]=='o'||temp[j]=='u'||temp[j]=='A'||
		temp[j]=='E'||temp[j]=='I'||temp[j]=='O'||temp[j]=='U')
		{
			 k=j;
			 while(k<len)
			 {
			 	temp[k]=temp[k+1];
			 	//cout<<temp[j]<<" ";
	            k++;
			 }
			 len--;
	    }
    }
    for(int j=0;j<len;j++)
    {
    	cout<<temp[j]<<" ";
	
    }
    
}
int main()
{
	string s1;
	cout<<"Enter the string\n";
	getline(cin,s1);
	del(s1);
}
