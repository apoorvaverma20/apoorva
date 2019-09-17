#include <stdio.h>
#include <string.h>
 
int main()
{
  	char str[100], oldch, Newch;
  	int i;
 
  	printf("\n Please Enter any String :  ");
  	gets(str);
  	
  	printf("\n Please Enter the Character that you want to Search for :  ");
  	scanf("%c", &oldch);
  	
  	getchar();
  	
  	printf("\n Please Enter the New Character :  ");
  	scanf("%c", &Newch);
  	
  	for(i = 0; i <= strlen(str); i++)
  	{
  		if(str[i] == oldch)  
		{
  			str[i] = Newch;
 		}
	}
	
	printf("\n The Final String after Replacing All Occurrences of '%c' with '%c' = %s ", oldch, Newch, str);
	
  	return 0;
}
