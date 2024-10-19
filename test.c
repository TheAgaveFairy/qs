#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "helpers.h"

#define SIZE 32
#define ln() printf("\n")
int main(){
    int i, *arr;
    srand(time(NULL));
    for(i = 0; i < 3; i++){
        arr = getTestArray(SIZE);
        printArray(arr, SIZE);
        insertion_sort(arr,SIZE/2,SIZE);
        printArray(arr, SIZE);
        insertion_sort(arr,0,SIZE/2);
        printArray(arr, SIZE);
        printf("%d. ", checkArray(arr, SIZE));
        free(arr);
        ln();
        sleep(2);
    }
    int *arr2 = getTestArray(SIZE);
    printArray(arr2, SIZE);
    //free(arr);
    free(arr2);
    return EXIT_SUCCESS;
}
