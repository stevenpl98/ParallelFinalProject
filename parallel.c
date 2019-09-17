#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mpi.h"
#include <omp.h>

void generate(int* arr, int n);
void copy(int * arr, int* arr2, int n);
void swap(int* arr, int i, int j);
void bubbleSort(int* arr, int n);
void oddEvenSort(int* arr, int n);
void print(int* arr, int n);

int main(int argc, char **argv)
{
    // initialize MPI  
	MPI_Init(&argc, &argv);

    double starttime,endtime;
    //int n= 25;//normal use
	int n = atoi(argv[1]);//used for cluster testing with './parallel.x 5' in which case n is 5

    int* arr= (int*)malloc(n * sizeof(int));
    int* arr2= (int*)malloc(n * sizeof(int));

    srand(time(NULL));
    #pragma omp parallel
    {
       generate(arr, n);
    }
    printf("Unsorted Array: ");
    print(arr, n);
    
    #pragma omp parallel
    {
       copy(arr, arr2, n);
    }
    printf("\nUnsorted Array Copy: ");
    print(arr2, n);
    printf("\n");

    printf("\nTESTING BUBBLE SORT\n******************************************************************************************************************\n");

    //tests bubble sort
    #pragma omp parallel
    {
	    starttime = omp_get_wtime();
    	bubbleSort(arr, n);
	    endtime = omp_get_wtime();
	    printf("Thread %d time is %f\n", omp_get_thread_num(),endtime-starttime);
    }
    printf("\nBubble Sort: ");
    print(arr, n);
    printf("\n\n");

    printf("\nTESTING ODD EVEN SORT\n******************************************************************************************************************\n");

    //tests odd even sort
    #pragma omp parallel
    {
       starttime = omp_get_wtime();
       oddEvenSort(arr2, n);
       endtime = omp_get_wtime();
       printf("Thread %d time is %f\n", omp_get_thread_num(), endtime-starttime);
    }

    printf("\nOdd Even Sort: ");
    print(arr2, n);

    free(arr);
    free(arr2);

    // done with MPI  
	MPI_Finalize();
    
    return 0;
}

//generates a random array used for sorting
void generate(int* arr, int n)
{
    //srand(time(NULL));
    #pragma omp for
    for(int i= 0; i < n; i++)
    {
        arr[i]= rand()%100;
    }
}

//makes a copy array for odd even sort to work with
void copy(int * arr, int* arr2, int n)
{
    #pragma omp for
    for(int i= 0; i < n; i++)
    {
        arr2[i]= arr[i];
    }
}

//swap function for all sorts
void swap(int* arr, int i, int j)
{
    int temp;
    temp= arr[i];
    arr[i]= arr[j];
    arr[j]= temp;
}

//executes bubble sort algorithm
void bubbleSort(int* arr, int n)
{
    for(int i= 0; i < n; i++)
    {
	int split = i%2;
	#pragma omp for
        for(int j = split; j < n - 1; j+=2)
        {
            if(arr[j] > arr[j+1])
	        {
                swap(arr, j, j+1);
	        }
        }
    }
}

//executes odd even sort algorithm
void oddEvenSort(int* arr, int n)
{
	for (int phase= 0; phase < n; phase++)
	{
		//even phase
		if (phase % 2 == 0)
		{
			#pragma omp for
			for (int i= 1; i < n; i += 2)
			{
				if (arr[i-1] > arr[i])
				{
                    swap(arr, i, i-1);
				}
			}
		}
		//odd phase
		else if(phase % 2 == 1)
		{
			#pragma omp for
			for (int i= 1; i < n; i += 2)
			{
				if (arr[i] > arr[i + 1])
				{
                    swap(arr, i, i+1);
				}
			}
		}
	}
}

//used to to test sorting
void print(int* arr, int n)
{
    for(int i= 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
}