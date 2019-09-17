#include <iostream>
using namespace std;

int main()
  {
  char c = 'n';
  cout << c<< endl               // output in ASCII
       << "OCT " << oct << (int)c << endl   // output in octal
       << "DEC " << dec << (int)c << endl   // output in decimal
       << "HEX " << hex << (int)c << endl;  // output in hexadecimal
  return 0;
  }
