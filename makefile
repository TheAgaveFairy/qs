CC = gcc
CFLAGS = -g -fopenmp
CPFLAGS = -g -pthread

all: quicksort quicksort_p

quicksort: quicksort.o helpers.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

quicksort_p: quicksort_p.o helpers.o
	$(CC) $(CPFLAGS) $(CFLAGS)  -o $@ $^

quicksort.o: quicksort.c quicksort.h helpers.h
	$(CC) $(CFLAGS) -c $< -lm

quicksort_p.o: quicksort_p.c quicksort_p.h helpers.h
	$(CC) $(CFLAGS) $(CFLAGS) -c $<

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f quicksort quicksort_p *.o
