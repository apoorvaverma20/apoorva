#include <iostream> 

using namespace std;

class complex_number
{
   public :
      int real, imag;
};

int main()
{
   complex_number num1, num2, sum;

   
   cout << "Enter real and imaginary parts of first complex number:"<<endl; 
   cin >> num1.real >> num1.imag;

   
   cout << "Enter real and imaginary parts of second complex number:"<<endl; 
   cin >> num2.real >> num2.imag;

 
   sum.real = num1.real + num2.real;
   sum.imag = num1.imag + num2.imag;

   if ( sum.imag >= 0 )
      cout << "Sum of two complex numbers = " << sum.real <<"+"<< sum.imag << "i"<<endl;
   else
     cout << "Sum of two complex numbers = " << sum.real << sum.imag << "i"<<endl;

   return 0;
}
