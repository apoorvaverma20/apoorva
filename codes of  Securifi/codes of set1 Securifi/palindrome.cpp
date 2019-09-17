86#include<bits/stdc++.h>
using namespace std;

int palind(int p)
{
	int x=p;
	int rem,t=0;
	while(x!=0)
	{
		rem=x%10;
		x=x/10;
		t=t*10+rem;
	}
	cout<<"Value of no found after solving is \n"<<t;
	if(p==t)
	{
		cout<<"\nNo.is a palindrome\n";
	}
	else
	{
		cout<<"\nNo.is not a palindrome\n";
	}
}

int main()
{
	int x;
	cout<<"Enter the value of x\n";
	cin>>x;
	palind(x);
}
