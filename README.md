# qs
 
This is our 4th(?) assignment due 19th Oct, '24.
Paul Dutton (hyf570) and Otto Zoesch-Weigel


Paul's Notes:
Claude showed me how to use rand_r for thread-safe rand number generation instead of rand when prompted for a code review. The data types it suggested were unecessarily large (long long to iterate through our array 2<<30 which should fit in an int). It then offered ways to parallelize the quicksort algorithm I sourced from https://github.com/portfoliocourses/c-example-code/blob/main/quicksort.c . 
