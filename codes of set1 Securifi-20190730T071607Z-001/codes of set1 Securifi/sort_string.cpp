#include<bits/stdc++.h> 
using namespace std; 
  
// function to print string in sorted order 
void sortString(string &str) 
{ 
   sort(str.begin(), str.end()); 
   cout << str; 
} 
 
int main() 
{ 
    string s = "apoorva";  
    sortString(s);  
    return 0; 
} 
