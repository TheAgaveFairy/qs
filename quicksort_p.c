#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "quicksort_p.h"

#define DEBUG 1 //if 1, will print human readable statements to stdout. if 0, outputs for redirection that will come to .csv
#define CUTOFF 1000

int ARRAY_SIZE;
int MAX_THREADS;

pthread_mutex_t thread_count_mutex = PTHREAD_MUTEX_INITIALIZER;
int thread_count = 1; // Start with the main thread

int main(int argc, char * argv[]){
	MAX_THREADS = atoi(argv[1]);
	ARRAY_SIZE = atoi(argv[2]);

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

    if(DEBUG) printf("Sorting completed in %f seconds\n", cpu_time_used);

    // Verify sorting (check first few and last few elements)
	if(DEBUG){
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
	}

	if(DEBUG) {
		int check = checkOnlyIncreasing(array, ARRAY_SIZE);
		if(check == 1) printf("Successfully sorted!!!");
		else printf("Not Successfully sorted!!!");
	}

	printf("%d, %d, %lf\n", thread_count, ARRAY_SIZE, cpu_time_used);

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

            pthread_mutex_lock(&thread_count_mutex);
            if (thread_count >= MAX_THREADS) {
                // Cannot create new threads, unlock mutex
                pthread_mutex_unlock(&thread_count_mutex);

                // Use sequential quicksort on both halves
                quicksort(arr, low, pivot - 1);
                quicksort(arr, pivot + 1, high);
            } else {
                // Increment thread_count
                thread_count += 2;
                pthread_mutex_unlock(&thread_count_mutex);

                // Create child threads
                pthread_create(&left_thread, NULL, parallel_quicksort, &left_params);
                pthread_create(&right_thread, NULL, parallel_quicksort, &right_params);

                // Wait for child threads to finish
                pthread_join(left_thread, NULL);
                pthread_join(right_thread, NULL);

                // Decrement thread_count
                pthread_mutex_lock(&thread_count_mutex);
                thread_count -= 2;
                pthread_mutex_unlock(&thread_count_mutex);
            }

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

//returns 1 if only increasing, returns 0 if not only increasing
int checkOnlyIncreasing(int *arr, int size){
	
	for(int i = 1; i < size; i++){
		if(arr[i - 1] > arr[i]){
			return 0;
		}
	}
	return 1;

}
