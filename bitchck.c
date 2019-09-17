#include<stdio.h>
int main()
{
FILE * f;
f = fopen("hex.txt","w");
fputc(1,f);
fputc(2,f);
fputc(3,f);
fputc(4,f);
return 0;
}
