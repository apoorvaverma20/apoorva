#include<bits/stdc++.h>
using namespace std;

float cel(float C)
{
	float K;
	K=C*(9/5)+32;
        return K;
}

float far(float F)
{
	float P,x,y;
         x=F-32;
         cout<<x<<"\n";
         y=(5.0/9.0);
        cout<<y<<"\n";
	P=x*y;
        cout<<P<<"\n";
        
	//cout<<"value after conversion from F to C is "<<P<<"\n";
        return P;
}
int main()
{
	float C,F,K,P;
	cout<<"Enter the value of Celcius \n";
	cin>>C;
	cout<<"Enter the value of Fahrenheit \n";
	cin>>F;
	K=cel(C);
        cout<<"value after conversion from C to F is "<<K<<"\n";
        P=far(F);
        cout<<"value after conversion from F to C is "<<P<<"\n";
}
