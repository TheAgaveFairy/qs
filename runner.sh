#!/bin/bash

run_test() {
	local p=$1
    local n=$2

	for i in {1..3}
	do
		./quicksort $1 $2  >> results/results_openmp.csv
	done
}

for p in {1,2,4,8,12}
do
    for i in {16..30}
    do
        n=$((2**i))
	    echo $p $n
	    run_test $p $n
    done
done

sed -i '1iThreads,ArraySize,Time' results/results_openmp.csv
