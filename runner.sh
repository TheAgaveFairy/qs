#!/bin/bash

run_test() {
	local p=$1

	for i in {1..20}
	do
		echo "$p" | ./quicksort >> results.csv
	done
}

for p in {1..12}
do
	echo $p
	run_test $p
done

sed -i '1iThreads,ArraySize,Quicksort' results.csv
