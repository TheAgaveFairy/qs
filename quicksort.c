#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#include "quicksort.h"
#include "helpers.h"

#define ARRAY_SIZE (INT_MAX / 2) //(INT_MAX / 2) smaller for testing
#define DEBUG 0 //if 1, will print human readable statements to stdout. if 0, outputs for redirection that will come to .csv
#define CUTOFF 1000 //Claude suggested parameter - probably going to dynamically calculate instead

int d_max = 0;
int cutoff = 0;
int main(int argc, char * argv[]){
    if(argc != 3){
        fprintf(stderr, "Please give the number of threads and array size. ex. for 12 threads and len 2^13./quicksort 12 8192");
        return EXIT_FAILURE;
    }
    /*
    if(DEBUG){
        printf("Array Size: %d\n", ARRAY_SIZE);
    } */   
    
    double temp = log(ARRAY_SIZE) / log(2);
    cutoff = (int)temp;
    if(DEBUG) printf("Cutoff log_2(ARRAY_SIZE): %d\n", cutoff);    

    int num_threads = atoi(argv[1]);
    int array_size = atoi(argv[2]);
   	
    //int num_threads = getNumThreads(DEBUG);
	omp_set_num_threads(num_threads);

	double start_time = omp_get_wtime();
    int *test_array = getTestArray(array_size);
	if(!test_array){
		fprintf(stderr, "Allocation failed. Exiting!");
		return EXIT_FAILURE;
	}
    double qs_begin_time = omp_get_wtime();

    //printArray(test_array, ARRAY_SIZE);
    quicksort(test_array, array_size);
    //printArray(test_array, ARRAY_SIZE);

	double end_time = omp_get_wtime();
    
    if(DEBUG){
        printf("%s\n", checkArray(test_array, array_size) ? "Sorted!" : "FAILURE");
	    printf("Max. Depth: %d\n", d_max);
        printf("Time for arraygen:\t%lf\nTime for quicksort:\t%lfs\nTotal time:\t\t%lf\n", qs_begin_time-start_time, end_time-qs_begin_time, end_time-start_time);
	} else {
        //printf("%d, %d, %lf, %lf, %lf\n", num_threads, ARRAY_SIZE, qs_begin_time-start_time, end_time-qs_begin_time, end_time-start_time);
        printf("%d, %d, %lf\n", num_threads, array_size, end_time-qs_begin_time);
    }
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
    d_max = depth > d_max? depth : d_max;
    if(low < high){
        if(depth > CUTOFF){ //do sequential
		    int pivot_index = partition(array, low, high);
		    quicksort_recursion(array, low, pivot_index - 1, depth+1);
		    quicksort_recursion(array, pivot_index +1, high, depth+1);		
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
        }
    }
}
int partition(int array[], int low, int high){
	int pivot_index = low + (rand() % (high-low)); //get a random index to use as pivot
	if(pivot_index != high){
		swap(&array[pivot_index], &array[high]);
	}
	
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
