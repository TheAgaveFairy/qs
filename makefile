CC = gcc
CFLAGS = -g -fopenmp

sumarray: quicksort.c quicksort.h
	$(CC) $(CFLAGS) -o quicksort quicksort.c

clean:
	rm -f quicksort
