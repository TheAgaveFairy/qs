# Quicksort Implementation with OpenMP and PThreads
 
## Paul Dutton (hyf570) and Otto Zoesch-Weigel

This is our 4th(?) assignment due 19th Oct, '24.

Paul's Notes:
Claude showed me how to use rand_r for thread-safe rand number generation instead of rand when prompted for a code review. The data types it suggested were unecessarily large (long long to iterate through our array 2<<30 which should fit in an int). It then offered ways to parallelize the quicksort algorithm I sourced from https://github.com/portfoliocourses/c-example-code/blob/main/quicksort.c . 

### Running the Program

All you need to do is decide if you want to run it manually (in which case you should change the DEBUG flag in quicksort.c to 1) or use the script "runner.sh" which will from 1-12 threads 20 times each to produce an output results.csv (with 240 lines) with the results. *********It will be 480 once I figure out how I want to handle having both programs, etc.


### This is a Claude household

No OpenAI found here!