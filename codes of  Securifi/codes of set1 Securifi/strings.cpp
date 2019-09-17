#include<bits/stdc++.h>
#include<iostream>
#include<string.h>
using namespace std;

int del(string str1)
{
	cout<<"DELETING WORD\n";
	string str2=str1.erase(10,3);
	cout<<str2;
}
int insert1(string str1)
{
	cout<<"INSERT\n";
 string	str2=str1.insert(7," now");
 cout<<str2<<"\n";
}
int add(string str1,string str2)
{
	cout<<"CONCATENATE\n";
	string str3=str1+str2;
    cout<<str3<<"\n";
	
}
int addmid(string str1,string str2)
{
	cout<<"ADDING IN MID\n";
    string str4=str1.substr(0,8)+str2;
    string str5=str1.substr(7,str1.length());
    
    cout<<str4<<" "<<str5<<"\n";
}
int sorting(string str5)
{
	cout<<"\nSORTING\n";
    sort(str5.begin(), str5.end() );
	cout<<str5;
	}

int main()
{
	string str1,str2,str5;
	str1="Welcome to this world";
	str2="come up";
	str5="APOORVA VERMA";
    add(str1,str2);
    addmid(str1,str2);
    insert1(str1);
    del(str1);
    sorting(str5);
	}
