#include<iostream>
#include<fstream>
//#include<conio.h>
#include<bits/stdc++.h>
using namespace std;
int main()
{
  int totrec=0;
  int choice;
  while(1)
  {
//clear();
cout<<"enter the choice\n";
cout<<"1) Entering intial records\n";
cout<<"2) Appending records\n";
cout<<"3) Viewing records\n";
cout<<"4) Exit\n";
cout<<"Enter your choice : ";
cin>>choice;
switch (choice)
{
  case 1 :   {
         ofstream outfile;
         outfile.open("stud.txt",ios::out);
         cout<<"\n\nPlease enter the details as per demanded\n";
         cout<<"\nEnter the name : ";
         char name[20];
         cin>>name;
         outfile<<name<<endl;
         cout<<"Enter Age : ";
         int age;
         cin>>age;
         outfile<<age<<endl;
         cout<<"Enter programming language known by him or her\n";
         char lang[25];
         cin>>lang;
         outfile<<lang<<endl;
         totrec= totrec + 1;
         outfile.close();
         }
         break;
  case 2 :   {
         ofstream outfile;
         outfile.open("stud.txt",ios::app); //app means append
         cout<<"\n\nPlease enter the details as per demanded\n";
         cout<<"\nEnter the name : ";
         char name[20];
         cin>>name;
         outfile<<name<<endl;
         cout<<"Enter Age : ";
         int age;
         cin>>age;
         outfile<<age<<endl;
         cout<<"Enter programming language known by him : ";
         char lang[25];
         cin>>lang;
         outfile<<lang<<endl;
         totrec = totrec + 1;
         outfile.close();
         }
         break;

  case 3 :   {
         ifstream infile;
         infile.open("stud.txt",ios::in);
         const int size=80;
         char line[size];
         int counter=totrec;
         while(counter > 0)
         {
         infile.getline(line,size);
         cout<<"\n\nNAME     : "<<line<<endl;
         infile.getline(line,size);
         cout<<"AGE      : "<<line<<endl;
         infile.getline(line,size);
         cout<<"LANGUAGE : "<<line<<endl;
         counter--;
         }
         infile.close();
         }
        // gets();
         break;
  case 4  :  
          cout<<"EXIT\n";
          break;
  default :  cout<<"\nInvalid Choice\nTRY AGAIN\n";
  }
}
}


