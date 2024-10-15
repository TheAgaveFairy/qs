#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdlib.h>
#include <string.h>

void swap(int *x, int *y);
void quicksort(int array[], int length);
void quicksort_recursion(int array[], int low, int high);
int partition(int array[], int low, int high);

int getNumThreads();
int getInt();

int getNumThreads(){	
	printf("How many threads do you want? (1 <= p <= 12): ");
	int num_threads = getInt();
	if (num_threads < 1 || num_threads > 12){
		return getNumThreads();
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

#endif
