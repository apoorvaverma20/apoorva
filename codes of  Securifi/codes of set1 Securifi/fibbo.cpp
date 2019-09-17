#include<bits/stdc++.h>
using namespace std;
int fibonacci(int n)
{
	int a=0,b=1,x;
	cout<<"Fibonacci series is\n";
	cout<<0<<" "<<1<<" ";
	for(int i=0;i<n-2;i++)
	{
		x=a+b;
		a=b;
		b=x;
        cout<<x<<" ";
 	}
}
int main()
{
	int n;
	cout<<"Enter the total values of series\n";
	cin>>n;
	fibonacci(n);
}
