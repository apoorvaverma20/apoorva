#include<iostream>
using namespace std;
int bubble(int arr[],int n)
{
	int temp;
	for(int i=0;i<n-1;i++)
	{
		cout<<"---Pass---"<<i+1<<endl;
	
		for(int j=0;j<n-i-1;j++)
		{
			if(arr[j]>arr[j+1])
			{
				temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
			for(int k=0;k<n;k++)
			cout<<arr[k]<<"  ";
			cout<<endl;
			cout<<endl;
		}
		
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
	bubble(arr,n);
}
