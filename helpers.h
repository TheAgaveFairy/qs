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
