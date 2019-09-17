#include<iostream>
using namespace std;
int main()
{
	int x,fact=1,*temp;
	cout<<"Enter the value of x\n";
	cin>>x;
	for(int i=x;i>=1;i--)
	{
		fact=fact*i;
	}
	temp=&fact;
	cout<<"Factorial of "<<x<<" is "<<*temp;
}
