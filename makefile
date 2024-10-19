CC = gcc
CFLAGS = -g -fopenmp
CPFLAGS = -g -pthread

all: quicksort quicksort_p

quicksort: quicksort.o helpers.o
	$(CC) $(CFLAGS) -o $@ $^

quicksort_p: quicksort_p.o helpers.o
	$(CC) $(CPFLAGS) -o $@ $^

quicksort.o: quicksort.c quicksort.h helpers.h
	$(CC) $(CFLAGS) -c $<

quicksort_p.o: quicksort_p.c quicksort_p.h helpers.h
	$(CC) $(CFLAGS) -c $<

helpers.o: helpers.c helpers.h
	$(CC) -c $<

clean:
	rm -f quicksort quicksort_p *.o
