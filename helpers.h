#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

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

int getNumThreads(int);
int getInt();
void printArray(int*, int);

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

