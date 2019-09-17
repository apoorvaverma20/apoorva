#include <stdio.h>
int main ()
{
  unsigned int x = 0x76543210;
  char *c = (char*) &x;
 
  printf ("*c is: 0x%x\n", *c);
  if (*c == 0x10)
  {
    printf ("It is little endian. \n");         //starts from back
  }
  else
  {
     printf ("It is big endian. \n");          //starts from front
  }
 
  return 0;
}
