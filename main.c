
#include <stdio.h>
#include <stdlib.h>

int main()
{
   FILE *fp;
   char str[60];

   /* opening file for reading */
   fp = fopen("setpointvalues.txt" , "r");
   if(fp == NULL) {
      perror("Error opening file");
      return(-1);
   }
  double h;
   char *ptr;
   while ( fgets (str, 60, fp) != NULL ) {
	printf("%s    \n",str);
	h = strtod(str,&ptr);
	h ++;
//	h =  strtod(str,NULL);
    printf("%f\n",h);
   }
   fclose(fp);
   
   return(0);
}
