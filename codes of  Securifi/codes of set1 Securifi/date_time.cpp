
#include<iostream> 
#include<ctime> 
using namespace std; 
int main() 
{ 
	time_t tt; 
	struct tm * ti; 
	time (&tt); 
	ti = localtime(&tt); 

	cout << "Current Day, Date and Time is = "
		<< asctime(ti); 

return 0; 
} 

