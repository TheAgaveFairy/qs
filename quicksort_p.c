#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "quicksort_p.h"
#include "helpers.h"

#define ARRAY_SIZE (INT_MAX / 64) //(INT_MAX / 2) smaller for testing
#define DEBUG 1 //if 1, will print human readable statements to stdout. if 0, outputs for redirection that will come to .csv
#define CUTOFF 8192 //size of [sub]array to put us in sequential mode
//DO WE NEED TO MAKE A THREAD NUMBER CUTOFF

int main(int argc, char * argv[]){
	/*
    int *array = malloc(sizeof(int) * ARRAY_SIZE);

	if(!array){
		fprintf(stderr, "Allocation failed. Exiting!");
		return EXIT_FAILURE;
	}

	if(DEBUG) printf("Initializing Array\n");
	srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand();
    }*/

    int *array = getTestArray(ARRAY_SIZE);
    //printArray(array, ARRAY_SIZE);
	if(DEBUG) printf("Array Initialized\n");

	clock_t start = clock();

    // Create initial sorting task
    SortParams *params = malloc(sizeof(SortParams));
    params->array = array;
    params->low = 0;
    params->high = ARRAY_SIZE;

    pthread_t initial_thread;
    pthread_create(&initial_thread, NULL, parallel_quicksort, params);
    fprintf(stderr, "XXXXXXXXXXXXXXXXXXXX");
    pthread_join(initial_thread, NULL);

    //if(debug) printArray(array, ARRAY_SIZE);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    if(DEBUG) printf("Sorting completed in %f seconds\n", cpu_time_used);
    
    if(DEBUG) printf("%s\n", checkArray(array, ARRAY_SIZE) ? "Sorted Correctly!" : "FAILURE TO SORT CORRECTLY!!!");
    free(params);
    free(array);
    return 0;
}

void *parallel_quicksort(void *arg) {
    SortParams *params = (SortParams *)arg;
    int *arr = params->array;
    int low = params->low;
    int high = params->high;
    fprintf(stderr, ".");
    if (low < high) {
        if (high - low < CUTOFF) {
            fprintf(stderr, "s\n");
            quicksort(arr, low, high); //sequential
        } else {
            int pivot = partition(arr, low, high);

            pthread_t left_thread, right_thread;
            SortParams left_params = {arr, low, pivot - 1};
            SortParams right_params = {arr, pivot + 1, high};

            if(pthread_create(&left_thread, NULL, parallel_quicksort, &left_params)) fprintf(stderr, "PThread L Create Failure");
            if(pthread_create(&right_thread, NULL, parallel_quicksort, &right_params)) fprintf(stderr, "PThread R Create Failure");
            fprintf(stderr,"c");
            pthread_join(left_thread, NULL);
            pthread_join(right_thread, NULL);
            fprintf(stderr,"j");
        }
    }
    
    return NULL;
}

void quicksort(int arr[], int low, int high) {
    //printArray(arr, ARRAY_SIZE);
    if (low < high) {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

int partition(int arr[], int low, int high) {
    int pivot_index = low + (rand() % (high - low));
    if (pivot_index != high){
        swap(&arr[pivot_index], &arr[high]);
    }

    int pivot = arr[high];
    int i = low; //Claude got weird with indexing, might've caused off by one errors
    int j;
    for (j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}
