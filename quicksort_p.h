#ifndef QUICKSORT_P_H
#define QUICKSORT_P_H

#include <stdlib.h>
#include <string.h>

void swap(int *x, int *y);
void quicksort(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void *parallel_quicksort(void *arg);

int getNumThreads(int);
int getInt();
void printArray(int*, int);

typedef struct {
    int *array;
    int low;
    int high;
} SortParams;

int getNumThreads(int debug){
    if(debug){	
	    printf("How many threads do you want? (1 <= p <= 12): ");
	}
    int num_threads = getInt();
	if (num_threads < 1 || num_threads > 12){
		return getNumThreads(debug);
	}
	return num_threads;
}

int getInt(){	
	char buf[16];
	int y;
	if (fgets(buf, sizeof(buf), stdin) != NULL){
		buf[strcspn(buf, "\n")] = 0;
		if(sscanf(buf, "%d", &y) == 1){
			return y;
		} else {
			return getInt();
		}
	}
	return -1;
}
void printArray(int *arr, int size){
    printf("Printing your array:\n");
    int i;
    for(i = 0; i < size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

#endif
