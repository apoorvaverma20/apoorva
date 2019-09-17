#include<bits/stdc++.h>
using namespace std;

int convert(string s1)
{
	char temp[100];
	for(int i=0;i<s1.length();i++)
	{
		temp[i]=s1.at(i);
	}
	for(int i=0;i<s1.length();i++)
{
	cout<<"ASCII value of "<<temp[i]<<" is "<<(int)temp[i]<<"\n";
}
}
int main()
{
	string s1;
	cout<<"Enter the string \n";
	getline(cin,s1);
	convert(s1);
}
