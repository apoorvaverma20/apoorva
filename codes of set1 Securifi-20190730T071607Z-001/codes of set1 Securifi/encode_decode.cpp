//write a program to encode and decode a string using base64 encode/decode method.

#include<bits/stdc++.h>
using namespace std;
int main()
{

	string sample = "India Team will win the Cup"; 
        cout<<"Sample String:\n"<<sample; 

	// Encode into Base64 format 
	string BasicBase64format = Base64.getEncoder().encodeToString(sample.getBytes()); 
        cout<<"Encoded String:\n"<< BasicBase64format; 
} 

