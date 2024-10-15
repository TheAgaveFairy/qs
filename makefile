CC = gcc
CFLAGS = -g -fopenmp

quicksort: quicksort.c quicksort.h quicksort_p.c quicksort_p.h
	$(CC) $(CFLAGS) -o quicksort quicksort.c
	$(CC) $(CFLAGS) -o quicksort_p quicsort_p.c

clean:
	rm -f quicksort quicksort_p
