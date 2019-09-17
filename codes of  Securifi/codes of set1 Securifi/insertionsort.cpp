#include<iostream>
using namespace std;
int insertion(int arr[],int n)
{
	int temp,key,j;
	for(int i=1;i<n;i++)
	{
		key=arr[i];
		cout<<"Pass "<<i<<endl;
		for(j=i-1;j>=0 && arr[j]<key;j--)
		{
		  arr[j+1]=arr[j];
    	}
	    arr[j+1]=key;
			
			for(int k=0;k<n;k++)
			cout<<arr[k]<<"  ";
			cout<<endl;
		}
}
int main()
{
	int arr[100],n;
	cout<<"Enter the total no of elements\n";
	cin>>n;
	cout<<"Enter the elements of array\n";
	for(int i=0;i<n;i++)
	cin>>arr[i];
	insertion(arr,n);
}
