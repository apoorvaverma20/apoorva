\#include<bits/stdc++.h>
using namespace std;
int mult(int k)
{
	int res;
	cout<<"Multiplication table of "<<k<<endl;
	for(int i=1;i<=10;i++)
	{
		res=k*i;
		cout<<k<<" * "<<i<<" = "<<res<<endl;
	}
}
int main()
{
	int x;
	cout<<"Enter the value of x\n";
	cin>>x;
	mult(x);
}
