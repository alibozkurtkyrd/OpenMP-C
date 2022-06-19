#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<omp.h>
#define array_size 100000  //this variable shows the number of floating point in the file. while reading file this variable will become the size of array(reading)

//gcc -fopenmp final.c -o final.o
int writeRandomNumber();

int main(int argc, char *argv[])
{
	//writeRandomNumber();//use this function to create random floating point ouput. After using it, commented this line because 
			    //	to compare the threads number floating point numbers should be equal. whenever this line works text.txt file regenerate
			    //	with different numbers. 
	//printf("%d floting points are written in test.txt file\n", array_size);

	//printf("The numbers inside test.txt file;\n");

   FILE* ptr = fopen("test.txt","r");
   if (ptr==NULL)
   {
      printf("There is no FILE");
      return 0;
   }
   //int histogram[5] = {0,0,0,0,0};

   int hist_0 = 0; 
   int hist_1 = 0;
   int hist_2 = 0;
   int hist_3 = 0;
   int hist_4 = 0;
   
   float read_array[array_size]; //the size of array size and the number of floating point in test.txt file should be equal
   float temp;
   int counter=0;
   while (fscanf(ptr, "%f",&temp) == 1)
   {
	//printf("temp degeri: %f", temp);
	read_array[counter] = temp;
   	counter++;
   }

   fclose(ptr);
	
  	double start;
  	double end;
	omp_set_dynamic(0);
	//omp_set_num_threads(1);

	// arguments of main function
	char *a = argv[1];
  	int threadNum = atoi(a);
  	printf("You have entered threadNum as  %d\n",threadNum);
	
	//int threadNum =8;
	
	start = omp_get_wtime();
 
	// I used reduction to prevent race condition between threads
	#pragma omp parallel for num_threads(threadNum) reduction(+:hist_0,hist_1,hist_2,hist_3,hist_4)	
   	for (int i=0; i < array_size;i++)
        {//itarete over array
	       // 	int histogram_private[5] = {0,0,0,0,0}; // histogram array causes to race condition. To prevent it new variable is created.	
		
		// it is better to use histogram with openmp reduction
     		if (read_array[i]>=0.0&&read_array[i]<1)
			hist_0++;
		else if (read_array[i]>=1.0&&read_array[i]<2)
			hist_1++;
		else if (read_array[i]>=2.0&&read_array[i]<3)
			hist_2++;
		else if (read_array[i]>=3.0&&read_array[i]<4)
			hist_3++;
		else if (read_array[i]>=4.0&&read_array[i]<5)
			hist_4++;
		    
		//printf("the number of threads: %d\n", omp_get_num_threads); 
		
		/*//this if block heps me to chekc whether 'i' variable equally distributed over threads or not 
		int ID = omp_get_thread_num();
	  	printf("My thread id is  %d\n", tid);
		

		if (ID==2)
		{
			printf("for i: %d\n",i);
			
		}
		*/

		//lets combine them
		
	}		
	end = omp_get_wtime();
	printf("For %d thread work took %f seconds\n", threadNum, (end - start));

    printf("[0-1] %d\n[1-2) %d\n[2-3) %d\n[3-4) %d\n[4-5) %d\n", hist_0,hist_1,hist_2,hist_3,hist_4);




	return 0;
}

int writeRandomNumber()
{//this function write random floating point number on test.txt file 
    srand((unsigned int)time(NULL));
    FILE *fp;

    fp = fopen("test.txt","w");
    float a = 5.0;
    for (int i=0;i<array_size;i++) // array_size definede above
    {    //sprintf(array[i],"%.2f\n", ((float)rand()/(float)(RAND_MAX)) * a);
	//array[i] = ((float)rand()/(float)(RAND_MAX)) * a;
    	fprintf(fp, "%f ", ((float)rand()/(float)(RAND_MAX)) * a);
    }
    fclose(fp);

    return 0;
}

