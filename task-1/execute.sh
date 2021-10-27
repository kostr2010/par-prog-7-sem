#!/bin/bash

if [[ -z "$1" ]]
	then
    		echo "no number of threads was given. defaults to 1"
		OMP_NUM_THREADS=1 ./a.out;
	else
		OMP_NUM_THREADS=$1 ./a.out;
fi
