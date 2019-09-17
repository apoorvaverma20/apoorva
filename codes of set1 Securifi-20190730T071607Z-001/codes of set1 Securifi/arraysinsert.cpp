#include<bits/stdc++.h>
using namespace std;
int start(int arr[],int n,int pos,int x)
{
	    for(int i=n;i>pos;i--)
	    {
	    	arr[i]=arr[i-1];
		}
			arr[pos]=x;
		for(int i=0;i<n+1;i++)
	    cout<<arr[i]<<" ";
}
	

int main()
{
	int pos,arr[100],n,x;
	cout<<"Enter the no of elements to be entered\n";
	cin>>n;
	cout<<"Enter the array list\n";
	for(int i=0;i<n;i++)
	cin>>arr[i];
	cout<<"Enter the position to be inserted\n";
	cin>>pos;
	cout<<"Enter the element to be inserted\n";
	cin>>x;
	start(arr,n,pos,x);

}
