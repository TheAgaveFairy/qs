#!/bin/bash

run_test() {
	local p=12
    local n=1073741824
    local c=$1

	for i in {1..3}
	do
		./quicksort $p $n $c >> results/results_cutoffs.csv
	done
}

for x in {64..128}
do
    c=$((2**x))
    echo $c
    run_test $c
done

sed -i '1iThreads,ArraySize,Cutoff,Time' results/results_cutoffs.csv
