#include<bits/stdc++.h>
using namespace std;
int main()
{
int arr[50],*p[50],n;
cout<<"Enter the size of array\n";
cin>>n;
cout<<"Enter the elements in array\n";
for(int i=0;i<n;i++)
cin>>arr[i];
for(int i=0;i<n;i++)
p[i]=&arr[i];

cout<<"Values in array are\n";
for(int i=0;i<n;i++)
cout<<*p[i]<<"\n";
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
}
