#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include <time.h>

#include "quicksort.h"

#define ARRAY_SIZE (INT_MAX / 2)

int main(int argc, char * argv[]){
	int *test_array = malloc(sizeof(int) * ARRAY_SIZE);
	if(!test_array){
		fprintf(stderr, "Allocation failed. Exiting!");
		return EXIT_FAILURE;
	}

	int num_threads = getNumThreads();
	omp_set_num_threads(num_threads);

	double start_time = omp_get_wtime();

	#pragma omp parallel
	{	
		unsigned int seed = time(NULL) ^ omp_get_thread_num();
		int i;
		#pragma omp for private(i)
		for(i = 0; i < ARRAY_SIZE; i++){
			test_array[i] = rand_r(&seed);
		}
	}
	/*
	for(i = 0; i < 10; i++){
		printf("%d, ", test_array[i]);
	}
	*/
	double end_time = omp_get_wtime();
	printf("Time taken: %lfs\n", end_time-start_time);
	
	free(test_array);
	return EXIT_SUCCESS;
}

/* taken from Kevin Browne github.com/portfoliocourses/c-example-code/ */
void swap(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}
void quicksort(int array[], int length){
	srand(time(NULL)); //could also manually set seed for reproducability
	quicksort_recursion(array, 0, length-1);
}
void quicksort_recursion(int array[], int low, int high){
	if(low < high){
		int pivot_index = partition(array, low, high);
		quicksort_recursion(array, low, pivot_index - 1);
		quicksort_recursion(array, pivot_index +1, high);		
	}
}
int partition(int array[], int low, int high){
	int pivot_index = low + (rand() % (high-low));
	if(pivot_index != high){
		swap(&array[pivot_index], &array[high]);
	}
	
	int p_value = array[high];
	
	int i, j;
	for (j = low; j < high; j++){
		if(array[j] <= p_value){
			swap(&array[i], &array[j]);
			i++;
		}
	}
	swap(&array[i], &array[high]);
	return i;
}
/* end copied code snippet */
