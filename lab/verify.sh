#!/bin/bash

set -e

for ((i = 1; i < 7; ++i)); do
	echo "$i proc"
	mpirun -n $i 1g-parallel.out
	diff result.txt result-par.txt
done

