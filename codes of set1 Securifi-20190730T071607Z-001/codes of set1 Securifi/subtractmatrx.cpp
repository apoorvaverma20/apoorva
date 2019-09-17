#include<bits/stdc++.h>
using namespace std;

int sub(int m1,int n1,int m2,int n2,int arr1[50][50],int arr2[50][50])
{
	int diff[50][50];
	for(int i=0;i<m1;i++)
    {
    	for(int j=0;j<n1;j++)
    	{
    		diff[i][j]=abs(arr1[i][j]-arr2[i][j]);
		}
	}
	cout<<"Sum of two arrays is\n";
	for(int i=0;i<m1;i++)
    {
    	for(int j=0;j<n1;j++)
    	{
    		cout<<diff[i][j]<<" ";
    	}
    	cout<<endl;
    }
	
}
int main()
{
	int arr1[50][50],arr2[50][50];
	int m1,n1,m2,n2;
	cout<<"Enter the total no of rows and columns of array1\n";
	cin>>m1>>n1;
	cout<<"Enter the total no of rows and columns of array2\n";
	cin>>m2>>n2;
	cout<<"Enter the values in array1\n";
	for(int i=0;i<m1;i++)
    {
    	for(int j=0;j<n1;j++)
    	cin>>arr1[i][j];
	}
	cout<<"Enter the values in array2\n";
	for(int i=0;i<m2;i++)
    {
    	for(int j=0;j<n2;j++)
    	cin>>arr2[i][j];
	}
	if(m1==m2 && n1==n2)
	sub(m1,n1,m2,n2,arr1,arr2);
	else
	cout<<"They can not be subtracted\n";
}
