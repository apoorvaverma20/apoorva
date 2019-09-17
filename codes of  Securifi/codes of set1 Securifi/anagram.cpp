#include<bits/stdc++.h>
using namespace std;
int anagram(string str1,string str2)
{
	int flag=0;
//	char str1[100],str2[100];
	int len1=str1.length();
	int len2=str2.length();
/*	for(int i=0;i<len1;i++)
	{
	str1[i]=str1.at(i);
	str2[i]=str2.at(i);
    }*/
 if(len1==len2)
    {
    sort(str1.begin(),str1.end());
    sort(str2.begin(),str2.end());
    for(int i=0;i<len1;i++)
    {
    if(str1[i]==str2[i])
    flag=0;
    else{
    flag=1;
    break;}
    }
    if(flag==0)
    cout<<"It is an anagram\n";
    else
    cout<<"It is not anagram\n";
    }
 else
    {
    	cout<<"It is not anagram\n";
	}   
}
int main()
{
	string str1,str2;
	cout<<"ENTER THE 1st STRING \n";
	getline(cin,str1);
	cout<<"ENTER THE 2nd STRING \n";
	getline(cin,str2);
	anagram(str1,str2);	
}
