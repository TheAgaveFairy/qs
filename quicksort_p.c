#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "quicksort_p.h"

#define CUTOFF 1000

int array_size;
int max_threads;
int max_threads_used = 1;
int debug = 0;

pthread_mutex_t thread_count_mutex = PTHREAD_MUTEX_INITIALIZER;
int thread_count = 1; // Start with the main thread

int main(int argc, char * argv[]){
	if(argc == 3){
		max_threads = atoi(argv[1]);
		array_size = atoi(argv[2]);
	} else if (argc == 4){
		max_threads = atoi(argv[1]);
		array_size = atoi(argv[2]);
		debug = atoi(argv[3]);
	}
	int *array = malloc(sizeof(int) * array_size);

	if(!array){
		fprintf(stderr, "Allocation failed. Exiting!");
		return EXIT_FAILURE;
	}

	if(debug) printf("Initializing Array\n");
	srand(time(NULL));
    for (int i = 0; i < array_size; i++) {
        array[i] = rand();
    }
	if(debug) printf("Array Initialized\n");

	clock_t start = clock();

    // Create initial sorting task
    SortParams *params = malloc(sizeof(SortParams));
    params->array = array;
    params->low = 0;
    params->high = array_size - 1;

    pthread_t initial_thread;
    pthread_create(&initial_thread, NULL, parallel_quicksort, params);
    pthread_join(initial_thread, NULL);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    if(debug) printf("Sorting completed in %f seconds\n", cpu_time_used);

    // Verify sorting (check first few and last few elements)
	if(debug){
		printf("First few elements: ");
		for (int i = 0; i < 5 && i < array_size; i++) {
			printf("%d ", array[i]);
		}
		printf("...\n");

		printf("Last few elements: ");
		for (int i = array_size - 5; i < array_size; i++) {
			printf("%d ", array[i]);
		}
		printf("\n");
	}

	if(debug) {
		int check = checkOnlyIncreasing(array, array_size);
		if(check == 1) printf("Successfully sorted!!!");
		else printf("Not Successfully sorted!!!");
	}

	printf("\n%d, %d, %lf\n", max_threads_used, array_size, cpu_time_used);

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
            if (thread_count >= max_threads) {
                // Cannot create new threads, unlock mutex
                pthread_mutex_unlock(&thread_count_mutex);

                // Use sequential quicksort on both halves
                quicksort(arr, low, pivot - 1);
                quicksort(arr, pivot + 1, high);
			} else if (thread_count >= max_threads - 1) {
				// Increment thread_count
                thread_count += 1;
                pthread_mutex_unlock(&thread_count_mutex);

                // Create child threads
                pthread_create(&left_thread, NULL, parallel_quicksort, &left_params);

                // Wait for child threads to finish
				quicksort(arr, pivot + 1, high);
                pthread_join(left_thread, NULL);

                // Decrement thread_count
                pthread_mutex_lock(&thread_count_mutex);

				if(thread_count > max_threads_used) max_threads_used = thread_count;

                thread_count -= 1;
                pthread_mutex_unlock(&thread_count_mutex);
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

				if(thread_count > max_threads_used) max_threads_used = thread_count;

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

/* insertion sort also taken from GitHub.com/PortfolioCourses/c-example-code */
void insertion_sort(int a[], int low, int high){
    int i;
    for(i = low + 1; i < high; i++){
        int key = a[i];
        int j = i - 1;
        while(j >= low && a[j] > key){
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = key;
    }
}