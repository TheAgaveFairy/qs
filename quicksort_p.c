#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "quicksort_p.h"

#define ARRAY_SIZE (INT_MAX / 16) //(INT_MAX / 2) smaller for testing
#define DEBUG 1 //if 1, will print human readable statements to stdout. if 0, outputs for redirection that will come to .csv
#define CUTOFF 12


int main(int argc, char * argv[]){
	int *array = malloc(sizeof(int) * ARRAY_SIZE);

	if(!array){
		fprintf(stderr, "Allocation failed. Exiting!");
		return EXIT_FAILURE;
	}

	if(DEBUG) printf("Initializing Array\n");
	srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand();
    }
	if(DEBUG) printf("Array Initialized\n");

	clock_t start = clock();

    // Create initial sorting task
    SortParams *params = malloc(sizeof(SortParams));
    params->array = array;
    params->low = 0;
    params->high = ARRAY_SIZE - 1;

    pthread_t initial_thread;
    pthread_create(&initial_thread, NULL, parallel_quicksort, params);
    pthread_join(initial_thread, NULL);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Sorting completed in %f seconds\n", cpu_time_used);

    // Verify sorting (check first few and last few elements)
    printf("First few elements: ");
    for (int i = 0; i < 5 && i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("...\n");

    printf("Last few elements: ");
    for (int i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);
    return 0;
}

void *parallel_quicksort(void *arg) {
    SortParams *params = (SortParams *)arg;
    int *arr = params->array;
    int low = params->low;
    int high = params->high;

    if (low < high) {
        if (high - low < CUTOFF) {
            quicksort(arr, low, high);
        } else {
            int pivot = partition(arr, low, high);

            pthread_t left_thread, right_thread;
            SortParams left_params = {arr, low, pivot - 1};
            SortParams right_params = {arr, pivot + 1, high};

            pthread_create(&left_thread, NULL, parallel_quicksort, &left_params);
            pthread_create(&right_thread, NULL, parallel_quicksort, &right_params);

            pthread_join(left_thread, NULL);
            pthread_join(right_thread, NULL);
        }
    }

    return NULL;
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}
