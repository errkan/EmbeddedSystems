#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
    //Initializing variables
   	//Change values of the pid/sampletime parameters here
	const double k_p = 4;
	const double k_i = 0.5;
	const double k_d =2;
	const double T_f = 4;
	const double K = 2;
	const double T = 3;
	const double h = 0.1; // sample time
	
	FILE *ifp,*ofp;
	
	char str[10];
	char file_name[25] = "setpointvalues.txt";
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
	double output_1;
	
double controller(double error){
		// Controller Model
		u_1 = k_p * error ;
		u_2 = u_2_1t + (k_i * h *error/2)+(k_i * h *error_1t/2);
		u_3 = (-(-T_f+h)*u_3_2t - 2*h*u_3_t - 2*k_d*error_2t + 2*k_d*error)/(2*T_f + h);
		u = u_1 + u_2 + u_3 ;
		
		u_2_1t = u_2;		
		
		u_3_2t = u_3_t;
		u_3_t = u_3;
				
		error_2t = error_1t;
		error_1t = error;
		
		return u;
}

double plant(double output, double u){
	output = (exp(-(h/T))*output)+(K*(1-exp(-(h/T)))*u);
	return output;
}

int main()
{
   printf("Enter the name of file you wish to read\n"); //Prompts for filename
   scanf("%s",file_name);//comment this line if you dont want the filename prompt
 
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
		
		u = controller(error);
		//printf("%e\n",u);		//Uncomment this line if you'd like to see the control signal
  		
  		//Plant Model
		output = plant(output,u);
		//Print to output.txt
		fprintf(ofp,"%e\n", output);
  }
 
   fclose(ifp);
   fclose(ofp);
   return 0;
}












