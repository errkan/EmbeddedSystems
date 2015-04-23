#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

sem_t s; // Declaration of Semaphore

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
double Input_reference[15];
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

int inputplace = -1;

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

//PID
void *thread1(void *arg)
{
	struct timespec sleepValue;
	sleepValue.tv_sec = 1;
	sleepValue.tv_nsec = 0;

	while (1) {
		sem_wait(&s);

		// To know the process running, this case PID
		//printf("Process running: %s", (char*)arg);

		/* critical section */
		if(inputplace > 13){
			printf("The program should end");
			fclose(ifp);
			fclose(ofp);
			exit(EXIT_SUCCESS);
		}
		error = Input_reference[++inputplace] - output;
		u = controller(error);
		/* critical section */

		sem_post(&s);
		nanosleep(&sleepValue, NULL);
	}
	return NULL;
}

// Plant
void *thread2(void *arg)
{
	struct timespec sleepValue;
	sleepValue.tv_sec = 0;
	sleepValue.tv_nsec = 100000000L;

	while (1) {
		sem_wait(&s);

		// To know the process running, this case Plant
		//printf("Process running: %s\n", (char*)arg);

		/* critical section */
		output = plant(output,u);
		fprintf(ofp,"%e\n", output);
		/* critical section */

		sem_post(&s);
		nanosleep(&sleepValue, NULL);
	}
	return NULL;
}

int main()
{
	//Getting the values from the folder

	printf("Enter the name of file you wish to read\n"); //Prompts for filename
    scanf("%s",file_name);//comment this line if you dont want the filename prompt

    ifp = fopen(file_name,"r"); // read from given filename
    ofp = fopen("output.txt","w"); // creates/rewrites output.txt file

    if( ifp == NULL )
    {
	   perror("Error while opening the file.\n"); //Close if file doesn't exist
	   exit(EXIT_FAILURE);
    }

    int loop = -1;
    while( fgets(str, 10, ifp) != NULL ) //Loop through the rows in the read file
    {
    	Input_reference[++loop] = strtod(str, &ptr);
    }

	pthread_t threadArray[2];	// array of thread IDs
	int status;		// error code
	pthread_attr_t threadAttribute;	// thread attribute

	// initialize semaphore s
	// First argument is the address to the semaphore
	// Second argument is if it is shared between threads in the same process (set to 0)
	// or between separate processes (set to 1).
	// Third argument is the initial value of the semaphore
	sem_init(&s, 0, 1);

	// initialize the thread attribute object
	status = pthread_attr_init(&threadAttribute);
	pthread_attr_setscope(&threadAttribute, PTHREAD_SCOPE_SYSTEM);

	// Create two threads and store their IDs in array threadArray

	char thread1Arg[] = "PID";
	pthread_create(&threadArray[0], &threadAttribute, thread1, (void *)&thread1Arg);

	char thread2Arg[] = "Plant";
	pthread_create(&threadArray[1], &threadAttribute, thread2, (void *)&thread2Arg);

	status = pthread_attr_destroy(&threadAttribute);	// destroy the attribute object


	// Wait for threads to finish
	status = pthread_join(threadArray[0], NULL);
	status = pthread_join(threadArray[1], NULL);

	// Destroy semaphore s
	sem_destroy(&s);
	return 0;
}
