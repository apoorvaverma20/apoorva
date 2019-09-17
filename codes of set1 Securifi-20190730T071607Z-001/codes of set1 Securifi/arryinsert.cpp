#include<bits/stdc++.h>
using namespace std;

int start(int arr[],int size,int pos,int insert)
{
	for(int i=size;i>pos;i--)
	{
		arr[i]=arr[i-1];
	}
	arr[pos]=insert;
	cout<<"New array is\n";
	for(int i=0;i<size+1;i++)
	{
		cout<<arr[i]<<" ";
	}
}
int main()
{
	int pos,insert,size,arr[50];
	cout<<"Enter the array size\n";
	cin>>size;
	cout<<"Enter the array elements\n";
	for(int i=0;i<size;i++)
	cin>>arr[i];
	cout<<"Enter the position to be inserted\n";
	cin>>pos;
	cout<<"Enter the element to be inserted\n";
	cin>>insert;
	
	start(arr,size,pos,insert);
}
