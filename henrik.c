#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int main()
{
   //Initializing variables
	char ch, file_name[25], str[10];
	FILE *ifp,*ofp;
	float f1;
	char *ptr;
	double u_3_2t = 0;
	double u_3_t = 0;
	double u_2_1t = 0;
	double error_2t=0;
	double error_1t=0;

	double u_1, u_2, u_3, u;
	double error=0;

	double output=0;

	double input;
	double output_1 ;
	//Change values of the pid/sampletime parameters here
	double k_p = 1;
	double k_i = 1;
	double k_d =1;
	double T_f = 1;
	double T = 1;
	double K = 1;
	double h = 0.1;
	
	double temp1,temp2;

 
   printf("Enter the name of file you wish to read\n");
   gets(file_name);
 
   ifp = fopen(file_name,"r"); // read from given filename
   ofp = fopen("output.txt","w"); // creates/rewrites output.txt file
 
   if( ifp == NULL )
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
 
   while( fgets(str, 10, ifp) != NULL )
  {
  		input = strtof(str, &ptr);
  		error = input - output ;

		// Controller Model
		u_1 = k_p * error ;
		u_2 = u_2_1t + (k_i * h *error/2)+(k_i * h *error_1t/2);
		u_3 = (1/((2*h) + (T_f*h*h)))*(-(u_3_t*2*T_f*h)-(u_3_2t*(T_f*h-(2*h)))+(4*k_d*(error-2*error_1t+error_2t)));
		u = u_1 + u_2 + u_3 ;

		u_2_1t = u_2;		
		
		u_3_2t = u_3_t;
		u_3_t = u_3;

		error_2t = error_1t;
		error_1t = error;
  		
  		//Plant Model

  		temp1 = (-(h/T));
  		temp2 = exp(temp1);
		output_1 = (temp2*output)+(K*(1-temp2)*u);
		output = output_1;
		
		//Print to output.txt
		fprintf(ofp,"%e\n", output);
  }
 
   fclose(ifp);
   fclose(ofp);
   return 0;
}












