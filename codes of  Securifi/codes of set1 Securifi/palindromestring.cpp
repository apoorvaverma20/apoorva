#include<bits/stdc++.h>
using namespace std;

int reverse(string s1)
{
	char temp[100],ori[100];
	int k=s1.length();
	int z=k,flag=1;
	for(int i=0;i<z;i++)
	{
		k--;
		temp[i]=s1.at(k);
		ori[i]=s1.at(i); //points to character in a string
		cout<<temp[i];
	}
	cout<<"\n";
	for(int j=0;j<z;j++)
	{
		cout<<ori[j];
	}
	for(int i=0;i<z;i++)
	{
		if(temp[i]==ori[i])
		{
			//cout<<"string is a palindrome";
			continue;
			
		}
		else
		{
			cout<<"\nString is not a palindrome";
			 break;
        }
 }
		    
		   
	}
/*	if(flag=1)
	cout<<"\nString is a palindrome";
	else
	cout<<"\nString is not a palindrome";*/

int main()
{
	string s1;
	cout<<"enter the string";
	getline(cin,s1);
	reverse(s1);
}
