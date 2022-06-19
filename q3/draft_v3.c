#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<omp.h>

// gcc -fopenmp draft_v3.c -o draft_v3.o -lm

int main(int argc, char *argv[])
{
    int n = 1000000 ;
    float x = 44.0f;


    float sum = 1.0f; // initialize sum of series

   	double start;
  	double end;
	omp_set_dynamic(0);
	//omp_set_num_threads(1);

	// arguments of main function
	char *a = argv[1];
  	int threadNum = atoi(a);
  	printf("You have entered threadNum as  %d\n",threadNum);


	start = omp_get_wtime();
	#pragma omp parallel for num_threads(threadNum) reduction(+:sum)	
    	for (int i = n - 1; i > 0; --i )
	{
		sum =  x * sum / i + 1 ;
		/*
		//this if block heps me to chekc whether 'i' variable equally distributed over threads or not
		int ID = omp_get_thread_num();
	  	//printf("My thread id is  %d\n", tid);

		
		if (ID==0)
		{
			printf("for i: %d\n",i);

		}
		*/
		
	}
   /*
    *  sum = 1 + x * 1/42	// üst: x^1  payda--> 42
    *  sum = 1 + x* (1+x*1/42)/41  //üst: x^2 payda --> 42.41
    *  sum = 1+x*[ 1 + x *[(1+x*1/42)/41]/40] //üst: x^3 paydıa --> 42.41.40
    *  ..
    *  sum = 1+x*[1+(x^n-1)*1/42)/41!   // x^n payda 42!
    */ 
    

	end = omp_get_wtime();
	printf("For %d thread work took %f seconds\n", threadNum, (end - start));

    	printf("e^%.0f = %f",x, sum);
   	 return 0;
}
