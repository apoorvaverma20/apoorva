#include<bits/stdc++.h>
using namespace std;
int swap(int *p,int *q)
{
	cout<<"p is "<<p<<endl<<"q is "<<q<<endl; //address
	int *temp;
	temp=p;
	p=q;
	q=temp;
	cout<<"p is "<<*p<<endl<<"q is "<<*q<<endl; //actual value
}
int main()
{
	int a,b,*p,*q;
	cout<<"Enter the values of a and b\n";
	cin>>a>>b;
	p=&a;
	q=&b;
	cout<<"p is "<<p<<endl<<"q is "<<q<<endl; //gives address of p and q
	swap(p,q);
}
