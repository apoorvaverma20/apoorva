#include<iostream>
#include<bits/stdc++.h>
#include<math.h>
using namespace std;
int details(string,int,string,string,int,float,float);
int calc(int,float,float);
int grades(float);

int details(string name,int roll_no,string sub1,string sub2,int total,float marks1,float marks2)
{
cout<<"Enter the name of a student\n";
 getline(cin,name);
 cout<<"Enter the roll no of a student\n";
 cin>>roll_no;
 cout<<"Enter the subjects\n";
 getline(cin,sub1);
 getline(cin,sub2);
 //cout<<"Enter the total marks\n";
 //cin>>total;
 cout<<"Enter the marks of a student in sub1\n";
 cin>>marks1;
 cout<<"Enter the marks of a student in sub2\n";
 cin>>marks2;
 return calc(total,marks1,marks2);
}
int calc(int total,float marks1,float marks2)
{
 float per;
 int total_obtained=marks1+marks2;
 cout<<"Total marks obtained is\n"<<total_obtained;
 per=total_obtained/2;
 cout<<"\npercentage obtained is\n"<<per<<"%\n";
 return grades(per);
}
int grades(float per)
{
 string grade;
 if(per>=90 && per<=100)
  grade="X";
 if(per>=75 && per<90)
  grade="A";
 if(per>=55 && per<75)
  grade='B';
 if(per>=40 && per<55)
  grade='C';
 if(per<40)
  grade='F';
 cout<<"Grade obtained is "<<grade<<"\n";
}

int main()
{
 int roll_no,total=100;
 string name;
 string sub1,sub2;
 float marks1,marks2;
 details(name,roll_no,sub1,sub2,total,marks1,marks2);
 //calc(total,marks1,marks2);
}

