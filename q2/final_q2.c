#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<omp.h>


// gcc -fopenmp final_q2.c -o final_q2.o
#define n 500 // (nx1) matris --> vector's row number
#define m 600 // 1xm matris --> column number
//m and n used to write matrices on file

int generateMatrixOutput();

int main(int argc, char *argv[])
{
 

  //generateMatrixOutput(); // to get a better amongs to different threads  result, call this function only once. after recalling it,comment this line
   
   FILE* ptr = fopen("q2.txt","r");
   if (ptr==NULL)
   {
      printf("There is no FILE");
      return 0;
   }

   int N, M;
   fscanf(ptr, "%*s %d",&N);//read the first line of q2.txt files N and M variable helps us to define matrices (states the size of array)
   fscanf(ptr, "%*s %d",&M);

   // printf("N ve M test: %d %d", N, M);
   //exit(-1);

   float a[N][1]; //this is (nx1) matris (vector)  
   float b[M]; // 1xm matris

   
//   float read_array[array_size];
   float temp;
   int counter=0;
   while (fscanf(ptr, "%f",&temp) == 1)
   {
	if (counter < N )// store the element of vector a
	{
		//printf("if temp degeri ve counter: %d  %f\n", temp,counter); 
		a[counter][0] = temp;
	}

	else // store the element of matris b
	{
		//printf("else temp degeri ve counter: %d  %f\n", temp,counter); 
		b[counter-N] = temp;  
	}
	counter++;
   }

   fclose(ptr);
  /* 
   printf("a vectorü: \n");
   for (int i = 0; i < N; i++)
   {
 	printf("%f\n ", a[i][0]);
   }
   
   printf("b arrayi:\n");
   for (int i = 0; i < M; i++)
   {
 	printf("%f\n ", b[i]);
   }
   */
	double start;
  	double end;
	omp_set_dynamic(0);

	// arguments of main function
	char *x = argv[1];
  	int threadNum = atoi(x);
  	printf("You have entered threadNum as  %d\n",threadNum);


   	float product[N][M];
	start = omp_get_wtime();
	//float sum=0;
	// İmportant this an perfectly nested loops which means we can use collapse
	// thanks to collapsing by using two loops I generated single loop with a new 
	// iteration  space (NXM)
	#pragma omp parallel num_threads(threadNum) 
	{

		#pragma omp for collapse(2) 
		for (int i=0; i < N; i++)
		{
			for (int j=0; j<M; j++)
			{
				product[i][j] = a[i][0] * b[j];
				
				/*
				//this if block heps me to chekc whether 'i' variable equally distributed over threads or not
				int ID = omp_get_thread_num();
	  			//printf("My thread id is  %d\n", ID);


				if (ID==0)
				{
					printf(" i,j: %d %d\n",i,j);

				}
				*/

			}

		}	
	}
	end = omp_get_wtime();
	printf("For %d thread work took %f seconds\n", threadNum, (end - start));

	
	ptr =fopen("q2_output.txt","w");
 	// this part should be printed on file rather than screen
	fprintf(ptr,"Result of product matrices\n\n");
	for (int i = 0; i < N ; i++)
	{
		for (int j = 0; j < M; j++)
		{
			fprintf(ptr, "%f ", product[i][j]);
		}
		fprintf(ptr,"\n");
	}
   	fclose(ptr);
}

int generateMatrixOutput()
{//this function write random floating point number between 0 and 'a' on q2.txt file
    srand((unsigned int)time(NULL));
    FILE *fp;

    fp = fopen("q2.txt","w");
    float a = 1000.0; // the biggest floating number (upper limit)

    fprintf(fp, "n: %d m: %d\n", n, m);// n and m will help while reading file
				       //
    for (int i=0; i<n+m; i++) // n and m variables defineded above
    {
    	    fprintf(fp, "%f ", ((float)rand()/(float)(RAND_MAX)) * a);
    }
    fclose(fp);
    printf("matrix and vector successfully writtten in to file\n");
    return 0;
}
