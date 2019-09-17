#include<bits/stdc++.h>
using namespace std;
int start(int arr[],int n,int pos)
{
	int temp[100];
	for(int i=n;i>=pos;i--)
	{
		temp[i-2]=arr[i-1];
	}
    for(int j=pos-1;j>=0;j--)
    {
    	temp[j]=arr[j];
	}
	cout<<"Array after deletion \n";
	for(int i=0;i<n-1;i++)
	cout<<temp[i]<<" ";
}
int main()
{
	int pos,n,arr[50];
	cout<<"Enter the array size\n";
	cin>>n;
	cout<<"Enter the array elements\n";
	for(int i=0;i<n;i++)
	cin>>arr[i];
	cout<<"Enter the position to be deleted\n";
	cin>>pos;
	
	start(arr,n,pos);
}
