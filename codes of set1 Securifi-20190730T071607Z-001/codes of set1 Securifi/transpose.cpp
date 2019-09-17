#include<bits/stdc++.h>
using namespace std;
int trans(int m,int n,int arr[50][50])
{
	int t[50][50];
	for(int i=0;i<n;i++)
    {
    	for(int j=0;j<m;j++)
    	{
    		t[i][j]=arr[j][i];
		}
    }
    cout<<"Transpose matrix is \n";
   	for(int i=0;i<n;i++)
    {
    	for(int j=0;j<m;j++)
    	{
    		cout<<t[i][j]<<"  ";
    	}
    	cout<<"\n";
    }
}
int main()
{
	int arr[50][50],m,n;
	cout<<"Enter the total no of rows and columns of array\n";
	cin>>m>>n;
	cout<<"Enter the values in array\n";
	for(int i=0;i<m;i++)
    {
    	for(int j=0;j<n;j++)
    	cin>>arr[i][j];
	}
	trans(m,n,arr);
}
