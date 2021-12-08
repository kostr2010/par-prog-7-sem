#!/bin/bash

set -e

echo "single-threaded"
time ./1g.out

for ((i = 1; i < 7; ++i)); do
	echo "$i-threaded"
	time mpirun -np $i 1g-parallel.out
done
