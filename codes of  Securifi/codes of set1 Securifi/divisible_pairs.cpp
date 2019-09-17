#include<iostream>
using namespace std;
int main()
{
 int a[100],n,x;
 cout<<"enter the value of n\n";
 cin>>n;
 cout<<"Enter the values in array\n";
 for(int i=0;i<n;i++)
 cin>>a[i];
 for(int i=0;i<n;i++)
 {
  for(int j=i+1;j<n;j++)
   {
    if(a[i]>a[j])
     x=a[i]%a[j];
    else
     x=a[j]%a[i];

   if(x==0)
   cout<<"pairs are "<<a[i]<<"  "<<a[j]<<"\n";
  }
 }
}
  
   
