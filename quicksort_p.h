#ifndef QUICKSORT_P_H
#define QUICKSORT_P_H

#include <stdlib.h>
#include <string.h>

void swap(int *x, int *y);
void quicksort(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void *parallel_quicksort(void *arg);
int checkOnlyIncreasing(int *arr, int size);
void insertion_sort(int a[], int low, int high);
int median_of_three(int arr[], int low, int high);

int getNumThreads(int);
int getInt();
void printArray(int*, int);

typedef struct {
    int *array;
    int low;
    int high;
} SortParams;

#endif
