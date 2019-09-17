1#include<bits/stdc++.h>
using namespace std;
int factor(int x)
{
	int i=2,r;
	cout<<"Factors of "<<x<<" are\n";
    while(x!=1)
	{
		if(x%i==0)
		{
		r=x/i;	
		x=r;
		cout<<i<<" ";
		i=2;
	    }
	    else
	    i++;
	}
}
int main()
{
	int x;
	cout<<"Enter the value of x\n";
	cin>>x;
	factor(x);
}
