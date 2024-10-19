CC = gcc
CFLAGS = -g -fopenmp
CPFLAGS = -g -pthread

quicksort: quicksort.c quicksort.h quicksort_p.c quicksort_p.h helpers.o
	$(CC) $(CFLAGS) -o quicksort quicksort.c
	$(CC) $(CPFLAGS) -o quicksort_p quicksort_p.c

helpers: helpers.c helpers.h
	$(CC) -o helpers.o helpers.c

clean:
	rm -f quicksort quicksort_p helpers.o
