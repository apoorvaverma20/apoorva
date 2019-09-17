#include <iostream> 
#include <sstream> 
using namespace std; 
  
int main() 
{ 
    string s = "12345"; 
  
    // object from the class stringstream 
    stringstream gk(s); 
  
    // The object has the value 12345 and give to x
    int x = 0; 
    gk >> x; 
  
   //variable x holds 12345 
    cout << "Value of x : " << x; 
  
    return 0; 
}
