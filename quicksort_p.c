#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "quicksort_p.h"

#define CUTOFF 24

int array_size;
int max_threads;
int debug = 0;

int main(int argc, char * argv[]){
    if(argc == 3){
        max_threads = atoi(argv[1]);
        array_size = atoi(argv[2]);
    } else if (argc == 4){
        max_threads = atoi(argv[1]);
        array_size = atoi(argv[2]);
        debug = atoi(argv[3]);
    } else {
        fprintf(stderr, "Usage: %s max_threads array_size [debug]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int *array = malloc(sizeof(int) * array_size);

    if(!array){
        fprintf(stderr, "Allocation failed. Exiting!\n");
        return EXIT_FAILURE;
    }

    if(debug) printf("Initializing Array\n");
    srand(time(NULL));
    for (int i = 0; i < array_size; i++) {
        array[i] = rand();
    }
    if(debug) printf("Array Initialized\n");

    struct timeval start, end;
    gettimeofday(&start, NULL);

    SortParams initial_params;
    initial_params.array = array;
    initial_params.low = 0;
    initial_params.high = array_size - 1;
    initial_params.depth = 0;  

    parallel_quicksort(&initial_params);

	gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) +
                        (end.tv_usec - start.tv_usec) / 1e6;

    if(debug) printf("Sorting completed in %f seconds\n", elapsed_time);

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

        int check = checkOnlyIncreasing(array, array_size);
        if(check == 1) printf("Successfully sorted!!!\n");
        else printf("Not Successfully sorted!!!\n");
    }

    printf("\n%d, %d, %lf\n", max_threads, array_size, elapsed_time);

    free(array);
    return 0;
}

void *parallel_quicksort(void *arg) {
    SortParams *params = (SortParams *)arg;

    if (params->low < params->high) {
        if (params->high - params->low < CUTOFF) {
            insertion_sort(params->array, params->low, params->high);
        } else {
            int pivot = partition(params->array, params->low, params->high);

            // Limit the depth of parallel recursion
            if (params->depth >= log2(max_threads)) {
                // Sequential quicksort
                quicksort(params->array, params->low, pivot - 1);
                quicksort(params->array, pivot + 1, params->high);
            } else {
                // Create parameters for child tasks
                SortParams left_params = {params->array, params->low, pivot - 1, params->depth + 1};
                SortParams right_params = {params->array, pivot + 1, params->high, params->depth + 1};

                pthread_t left_thread;
                int thread_created = 0;

                // Try to create a new thread for the left partition
                if (params->depth < log2(max_threads)) {
                    if (pthread_create(&left_thread, NULL, parallel_quicksort, &left_params) == 0) {
                        thread_created = 1;
                    }
                }

                // Recurse on the right partition in the current thread
                parallel_quicksort(&right_params);

                if (thread_created) {
                    pthread_join(left_thread, NULL);
                } else {
                    // If thread creation failed, process left partition sequentially
                    parallel_quicksort(&left_params);
                }
            }
        }
    }
    return NULL;
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        if (high - low < CUTOFF) {
            insertion_sort(arr, low, high);
        } else {
            int pivot = partition(arr, low, high);
            quicksort(arr, low, pivot - 1);
            quicksort(arr, pivot + 1, high);
        }
    }
}

int partition(int arr[], int low, int high) {
    int median_index = median_of_three(arr, low, high);

    swap(&arr[median_index], &arr[high]);

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

int median_of_three(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    int a = arr[low];
    int b = arr[mid];
    int c = arr[high];

    if ((a <= b && b <= c) || (c <= b && b <= a))
        return mid;
    else if ((b <= a && a <= c) || (c <= a && a <= b))
        return low;
    else
        return high;
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Returns 1 if only increasing, returns 0 if not only increasing
int checkOnlyIncreasing(int *arr, int size){
    for(int i = 1; i < size; i++){
        if(arr[i - 1] > arr[i]){
            return 0;
        }
    }
    return 1;
}

void insertion_sort(int a[], int low, int high){
    int i;
    for(i = low + 1; i <= high; i++){
        int key = a[i];
        int j = i - 1;
        while(j >= low && a[j] > key){
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = key;
    }
}

