#include<bits/stdc++.h>
using namespace std;

int Ch(char character[100])
{
	int i=0;
	for(char j='A';j<='Z';j++)
	{
		character[i]=j;
		i++;
	}
}
int As(int ascii[100])
{
	int i=0;
	for(int j=65;j<91;j++)
	{
		ascii[i]=j;
		i++;
	}
}
int He(int hex[100])
{
	int i=0,k=15;
	for(int j=41;j<=49;j++)
	{
		hex[i]=j;
		i++;
	}
	for(int l=50;l<=59;l++)
	{
		hex[k]=l;
		k++;
	}
}
int print(char character[100],int ascii[100],int hex[100])
{

	for(int i=0;i<26;i++)
	{
	    	
			if(character[i]=='J')
		    	cout<<"   "<<character[i]<<"     "<<ascii[i]<<"     "<<"4A"<<"\n";
			if(character[i]=='K')
				cout<<"   "<<character[i]<<"     "<<ascii[i]<<"     "<<"4B"<<"\n";
			if(character[i]=='L')
			    cout<<"   "<<character[i]<<"     "<<ascii[i]<<"     "<<"4C"<<"\n";
			if(character[i]=='M')
				cout<<"   "<<character[i]<<"     "<<ascii[i]<<"     "<<"4D"<<"\n";
			if(character[i]=='N')
				cout<<"   "<<character[i]<<"     "<<ascii[i]<<"     "<<"4E"<<"\n";
			if(character[i]=='O')
			    cout<<"   "<<character[i]<<"     "<<ascii[i]<<"     "<<"4F"<<"\n";
			if(character[i]=='Z')
			    cout<<"   "<<character[i]<<"     "<<ascii[i]<<"     "<<"5A"<<"\n";
		    if(i<9||i>=15)
			cout<<"   "<<character[i]<<"     "<<ascii[i]<<"     "<<hex[i]<<"\n";

	}
	
}

int main()
{
	char character[100];
	int ascii[100],hex[100];
	cout<<"|--Character--|"<<"--ASCII Dec--|"<<"--ASCII Hex--|"<<endl;
	Ch(character);
	As(ascii);
	He(hex);
	print(character,ascii,hex); 
}
