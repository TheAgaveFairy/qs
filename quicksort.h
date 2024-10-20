#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdlib.h>
#include <string.h>

void swap(int *x, int *y);
void quicksort(int array[], int length);
void quicksort_recursion(int array[], int low, int high, int depth);
int partition(int array[], int low, int high);
int medianOfThree(int *arr, int low, int high);

#endif
