#!/bin/bash

run_test() {
	local p=12
    local n=1073741824
    local c=$1

	for i in {1..5}
	do
		./quicksort $p $n $c >> results/cutoffs.csv
	done
}

for x in {64..128..8}
do
    c=$((2**x))
    echo $x
    run_test $x
done

sed -i '1iThreads,ArraySize,Cutoff,Time' results/cutoffs.csv
