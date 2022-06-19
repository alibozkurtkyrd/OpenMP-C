// heat matrisi olusturuldu v2.c de bunu bir methoda al
// ayrıt methoda alma başarısız oldu mainde yapacagım bu işlemi

//gcc -fopenmp v7.c -o v7.o -lm  

// sadece write işlemi yapacagım
//
// ONEMLI: input dosyasındaki sadece N ve ayrıca ısıtıcıların degeride degistirilibilr(x,y ve power gibi) ANCAK heatNumber(ıstıcı sayısı) degistirilMEmelidir. 
	// UYARI ısıtıcların x ve y degerleri N den kücük olmalıdır
// cünkü 10 adet heat struct yapısı olusturulmuştur
#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include<omp.h>

struct heater{
  int x;
  int y;
  int power;
};
float distance(int x1, int y1, int x2, int y2);
struct heater findClosestHeater(int x1, int y1, struct heater h_array[], int length);


struct square{
  int x;
  int y;

};
int main(int argc, char *argv[])
{

	float heat[7][7];
	
	
	// section1 --> heat array i olusturuldu
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (i==0 && j==0)
				heat[i][j] = 1; // ısı kaynagı ile nokta çakışırsa soruda verilmemiş ancak ben enerjinin tümünü aktardıgını kabul ettim

			else if (i <3 && j <3)
			{
				if (i==2 && j==2)
				{
					heat[i][j] = 0.75; // i ve j degeri 3 den kücük iken sarı renk ancak tek istisna i ve j nin 2 olma durumları bu durumda renk kırmızı
				}
				else
					heat[i][j] = heat[j][i] =  0.85;
			}

			else if (i<5 && j<5)
			{ 
				if (i==4 && j==4)
				{
				    heat[i][j] = 0.65; // i vej 4 iken tek bu durumda açık mavi digerlerin kırmızı(3 ten büyük iken)
				}

				else	
					heat[i][j] = heat[j][i] =  0.75;
			}

			else if (i<6 && j<6)
			{ 
				if (i==5 && j==5)
				{
				    heat[i][j]= 0.6; // i vej 5 e esit olma durumu tek bu durumda koyu  mavi digerlerin açık mavi
				}	
				else 
					heat[i][j] = heat[j][i] = 0.65;
			}

			else if (i<7 && j<7)
			{ 
				if (i==6 && j==6)
				{
				    heat[i][j]= 0; // bu noktada siyah 
				}	
				else
					heat[i][j] = heat[j][i] = 0.6;
			}
		}

	}

	
	//  SECTION4 read işlemi

	   FILE* ptr = fopen("input2.txt","r");
   	   if (ptr==NULL)
  	   {
      		printf("There is no FILE");
      		return 0;
   	   }
	int N,P;
	fscanf(ptr, "%*s %d",&N);//read the N	
	fscanf(ptr, "%*s %d",&P);//read the P the number of heaters

//	printf("N ile P : %d, %d",N,P);	
	float plate[N][N];

	struct heater h_array[P];// ısıtıcıları depolayan array

	int temps[50];
	int counter = 0;
	int kk;
	 while (!feof (ptr))
  	 {	fscanf(ptr, "%d",&kk); 
		//printf("temp degeri: %f", temp);
		temps[counter] = kk;
   		counter++;
   		}	

   	fclose(ptr);

	// dosya içerisinde 10 adet ıstıcı oldugu belirtildigi için heater den 10 adet obje olusturuldu
	struct heater h1;
	h1.x = temps[0];  h1.y =temps[1];  h1.power = temps[2];
	struct heater h2;
	h2.x = temps[3];  h2.y =temps[4];  h2.power = temps[5];
	struct heater h3;
	h3.x = temps[6];  h3.y =temps[7];  h3.power = temps[8];
	struct heater h4;
	h4.x = temps[9];  h4.y =temps[10];  h4.power = temps[11];
	struct heater h5;
	h5.x = temps[12];  h5.y =temps[13];  h5.power = temps[14];
	struct heater h6;
	h6.x = temps[15];  h6.y =temps[16];  h6.power = temps[17];
	struct heater h7;
	h7.x = temps[18];  h7.y =temps[19];  h7.power = temps[20];
	struct heater h8;
	h8.x = temps[21];  h8.y =temps[22];  h8.power = temps[23];
	struct heater h9;
	h9.x = temps[24];  h9.y =temps[25];  h9.power = temps[26];
	struct heater h10;
	h10.x = temps[27];  h10.y =temps[28];  h10.power = temps[29];

	// assing each struct to array
	h_array[0]= h1; h_array[1] = h2; h_array[2] = h3; h_array[3] = h4; h_array[4] = h5; h_array[5] = h6; h_array[6] = h7;
	h_array[7]= h8;	h_array[8]= h9;	h_array[9]= h10;

	// hesaplamalar 
	double start;
  	double end;
	omp_set_dynamic(0);

	// arguments of main function
	char *x = argv[1];
  	int threadNum = atoi(x);
  	printf("You have entered threadNum as  %d\n",threadNum);
	

	struct heater temp2; 
	
	start = omp_get_wtime();
	
   #pragma omp parallel num_threads(threadNum) 
    {
	// her bir nokta icin en yakın ısıtıcıyı bulup o noktanın sıcaklıgını bulmuş olacagım
	
	#pragma omp for collapse(2)
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
		{

			temp2 = findClosestHeater(i,j,h_array,P);// en yakın ısıtıcıyı buluyoruz
			if (abs(j-temp2.y)>6 || abs(i-temp2.x)>6)
			{// en yakın ıstıcıdan ısı alamaz yani o nokta 0 derecedir
				plate[i][j] = 0;
			}			

			else{
			// en yakın ısıtıcının 
				plate[i][j] = heat[abs(j-temp2.y)][abs(i-temp2.x)]*temp2.power; // heat (x,y ve power gibi)array indeki degeri en yakın ısıtcının gücü ile çarpıyorum
			}	
		}
	}

    }
    end = omp_get_wtime();
    printf("For %d thread work took %f seconds\n", threadNum, (end - start));
	
    ptr =fopen("output2.txt","w");
    // this part should be printed on file rather than screen
    fprintf(ptr,"Result of product matrices\n\n");
    for (int i = 0; i < N ; i++)
    {
	for (int j = 0; j <N ; j++)
	{
		fprintf(ptr, "%f ", plate[i][j]);
	}
	fprintf(ptr,"\n");
    }
   	fclose(ptr);
}

float distance(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

struct heater findClosestHeater(int x1, int y1, struct heater h_array[], int length)
{// bu fonksiyon struct array i içerisindeki en kısa mesafeye sahip struct i döndürüyor
	struct heater tempStruct = h_array[0];
	float temp;
	float smallest = distance(x1,y1,h_array[0].x,h_array[0].y); 
	for (int i= 1; i<length; i++)
	{
		temp = distance(x1,y1,h_array[i].x,h_array[i].y);
		//printf("temp degeri: %f array[%d] icin:\n",temp,i);
		if (temp< smallest)
		{
			smallest = temp;
			tempStruct = h_array[i];
		}
	}

//	printf("x: %d ve y: %d noktarı icin en kısa mesafe:%f strcut(x:%d,y:%d) power : %d\n",x1,y1,smallest,tempStruct.x,tempStruct.y, tempStruct.power);
	return tempStruct;

}
