#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#include "quicksort.h"
#include "helpers.h"

#define DEBUG 0 //if 1, will print human readable statements to stdout. if 0, only prints stats (mainly for redirection to a .csv)

//int d_max = 0;  // counts how deep we are in recursion

int array_size;
int cutoff = 64; //values 20,32,64,1024 have been tested. lower range is better!

int main(int argc, char * argv[]){
    if(argc < 3 || argc > 4){
        fprintf(stderr, "Please give the number of threads, array size, and an optional cutoff. ex. for 12 threads and len 2^13 and cutoff 32 ./quicksort 12 8192 32");
        return EXIT_FAILURE;
    }
    if(argc == 4) cutoff = atoi(argv[3]);
   
    int num_threads = atoi(argv[1]); //not super safe but ok for today
    array_size = atoi(argv[2]);
    
    if(DEBUG) printf("Cutoff : %d\n", cutoff);    
	
	omp_set_num_threads(num_threads);

	double start_time = omp_get_wtime();
    int *test_array = getTestArray(array_size);
	if(!test_array){
		fprintf(stderr, "Allocation failed. Exiting!");
		return EXIT_FAILURE;
	}

    double qs_begin_time = omp_get_wtime();
    //insertion_sort(test_array, 0, array_size);
    quicksort(test_array, array_size);
	double end_time = omp_get_wtime();
    
    if(DEBUG){
        printf("%s\n", checkArray(test_array, array_size) ? "Sorted!" : "FAILURE");
	    //printf("Max. Depth: %d\n", d_max);
        printf("Time for arraygen:\t%lf\nTime for quicksort:\t%lfs\nTotal time:\t\t%lf\n", qs_begin_time-start_time, end_time-qs_begin_time, end_time-start_time);
	}
    printf("%d, %d, %d, %lf\n", num_threads, array_size, cutoff, end_time-qs_begin_time);
    
	free(test_array);
	return EXIT_SUCCESS;
}

/* taken from Kevin Browne github.com/portfoliocourses/c-example-code/ with modifications suggested by Claude */
void swap(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}
void quicksort(int array[], int length){
    srand(time(NULL)); //could also manually set seed for reproducability
    #pragma omp parallel
    {        
        #pragma omp single nowait
        {
            quicksort_recursion(array, 0, length-1,0);
        }    
    }
}
void quicksort_recursion(int array[], int low, int high, int depth){
    //d_max = depth > d_max? depth : d_max;
    if(low < high){
        if((high - low) < cutoff){ //do sequential
            insertion_sort(array, low, high+1);
        } else {
		    int pivot_index = partition(array, low, high);
            #pragma omp task
            {
		        quicksort_recursion(array, low, pivot_index - 1, depth + 1);
		    }
            #pragma omp task
            {
                quicksort_recursion(array, pivot_index+1, high, depth + 1);
            }
            #pragma omp taskwait // might not be strictly needed
        }
    }
}
int partition(int array[], int low, int high){
	/*int pivot_index = low + (rand() % (high-low)); //get a random index to use as pivot
	if(pivot_index != high){
		swap(&array[pivot_index], &array[high]);
	}
    OLD DEPRECATED RANDOM PIVOT */
    int pivot_index = medianOfThree(array, low, high);
	
    int pivot_value = array[high];
	
	int i = low;
    int j;
	for (j = low; j < high; j++){
		if(array[j] <= pivot_value){
			swap(&array[i], &array[j]);
			i++;
		}
	}
	swap(&array[i], &array[high]);
	return i;
}
/* end copied code snippet */
int medianOfThree(int *arr, int low, int high){
    int mid = low + (high - low) / 2; // prevent overflow
    if(arr[low] > arr[mid])
        swap(&arr[low], &arr[mid]);
    if(arr[low] > arr[high])
        swap(&arr[low], &arr[high]);
    if(arr[mid] > arr[high])
        swap(&arr[mid], &arr[high]);
    //might as well order them as we go

    swap(&arr[mid], &arr[high]);
    return high;
}
