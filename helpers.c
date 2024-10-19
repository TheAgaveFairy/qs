#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "helpers.h"

int * getTestArray(int size){
    int *test_array = malloc(sizeof(int) * size);
	
	#pragma omp parallel
	{	
		unsigned int seed = time(NULL) ^ omp_get_thread_num();
		int i;
		#pragma omp for private(i)
		for(i = 0; i < size; i++){
			test_array[i] = rand_r(&seed); // can %100 for example to make it human readable during testing
		}
	}
    return test_array;
}

int getNumThreads(int debug){
    if(debug){	
	    printf("How many threads do you want? (1 or more): ");
	}
    int num_threads = getInt();
	if (num_threads < 1){
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
int checkArray(int *arr, int size){
    int i;
    for(i = 1; i < size; i++){
        if(arr[i] < arr[i-1]){
            fprintf(stderr, "WOAH! %d, %d!", arr[i-1], arr[i]);
            return 0;
        }
    }
    return 1;
}
