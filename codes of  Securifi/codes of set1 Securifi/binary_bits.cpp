#include<iostream>
#include<bits/stdc++.h>
using namespace std;
int main()
{
 int x,rem,count=0,bit[100],i=1,pos,k=0;
 cout<<"Enter the value of x\n";
 cin>>x;
// cout<<"Enter the position to clear\n";
 //cin>>pos;
 while(x!=0)
 {
  rem=x%2;
  x=x/2;
  bit[i]=rem;
  k++;
  cout<<bit[i]<<" ";
  if(rem==1)
  count++;
  i++;
 }
 cout<<"No.of set bits is "<<count<<"\n";
 
}
 
