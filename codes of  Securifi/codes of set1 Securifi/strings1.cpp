
#include<iostream>
using namespace std;
int main()
{
	string str1="hello  ";
	string str2="everyone";
    string str3=str1+str2;
    string str4;
    cout<<"enter your name";
    getline(cin,str4);
    cout<<str3;
    cout<<"\n"<<str4;
}
