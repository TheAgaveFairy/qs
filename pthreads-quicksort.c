#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 8
#define CUTOFF 1000

typedef struct {
    int *array;
    int low;
    int high;
} SortParams;

void quicksort(int arr[], int low, int high);
void *parallel_quicksort(void *arg);
int partition(int arr[], int low, int high);
void swap(int* a, int* b);

int main() {
    int length = 10000000;  // 10 million elements
    int *array = malloc(sizeof(int) * length);
    
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize array with random values
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        array[i] = rand() % length;
    }

    printf("Array initialized. Starting sort...\n");

    clock_t start = clock();

    // Create initial sorting task
    SortParams *params = malloc(sizeof(SortParams));
    params->array = array;
    params->low = 0;
    params->high = length - 1;

    pthread_t initial_thread;
    pthread_create(&initial_thread, NULL, parallel_quicksort, params);
    pthread_join(initial_thread, NULL);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Sorting completed in %f seconds\n", cpu_time_used);

    // Verify sorting (check first few and last few elements)
    printf("First few elements: ");
    for (int i = 0; i < 5 && i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("...\n");

    printf("Last few elements: ");
    for (int i = length - 5; i < length; i++) {
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

    free(params);
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
