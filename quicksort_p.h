#ifndef QUICKSORT_P_H
#define QUICKSORT_P_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    int *array;
    int low;
    int high;
} SortParams;

void swap(int *x, int *y);
void quicksort(int arr[], int low, int high);
void * parallel_quicksort(void *);
void quicksort_recursion(int array[], int low, int high);
int partition(int array[], int low, int high);

#endif
