#include<bits/stdc++.h>
using namespace std;

int power1(int a,int b)
{
	int x=1;
	for(int i=1;i<=b;i++)
	{
		x=x*a;
	}
	cout<<"Value found when "<<b<<"is a power of "<< a<<" is "<<x<<endl;
}
int main()
{
	int a,b;
	cout<<"Enter the value of a and b\n";
	cin>>a>>b;
	power1(a,b);
}
