#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int main()
{
   //Initializing variables
   	//Change values of the pid/sampletime parameters here
	double k_p = 1;
	double k_i = 1;
	double k_d =1;
	double T_f = 1;
	double K = 1;
	double T = 1;
	double h = 0.1; // sample time
	
	FILE *ifp,*ofp;
	
	char ch, file_name[25], str[10];
	char *ptr;
	float f1;
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

   printf("Enter the name of file you wish to read\n"); //Prompts for filename
   //fgets(file_name, sizeof(file_name), stdin);
   scanf("%s",file_name);
 
   ifp = fopen(file_name,"r"); // read from given filename
   ofp = fopen("output.txt","w"); // creates/rewrites output.txt file
 
   if( ifp == NULL )
   {
      perror("Error while opening the file.\n"); //Close if file doesn't exist
      exit(EXIT_FAILURE);
   }
 
   while( fgets(str, 10, ifp) != NULL ) //Loop through the rows in the read file
  {
  		input = strtod(str, &ptr);
  		error = input - output ;

		// Controller Model
		u_1 = k_p * error ;
		u_2 = u_2_1t + (k_i * h *error/2)+(k_i * h *error_1t/2);
		//u_3 = (1/((2*h) + (T_f*h*h)))*(-(u_3_t*2*T_f*h)-(u_3_2t*(T_f*h-(2*h)))+(4*k_d*(error-2*error_1t+error_2t)));
		u_3 = (-(-T_f+h)*u_3_2t - 2*h*u_3_t - 2*k_d*error_2t + 2*k_d*error)/(2*T_f + h);
		u = u_1 + u_2 + u_3 ;

		u_2_1t = u_2;		
		
		u_3_2t = u_3_t;
		u_3_t = u_3;

		error_2t = error_1t;
		error_1t = error;
		
		
  		
  		//Plant Model
		output_1 = (exp(-(h/T))*output)+(K*(1-exp(-(h/T)))*u);
		output = output_1;
		
		//Print to output.txt
		fprintf(ofp,"%e\n", output);
  }
 
   fclose(ifp);
   fclose(ofp);
   return 0;
}












