#!/bin/bash

N_THREADS=1

if [[ -z "$1" ]]
	then
		echo "no number of threads was given. defaults to 1"
	else
		N_THREADS=$1;
fi

if [[ -z "$2" || -z "$3" || -z "$4" || -z "$5" ]]
	then
		echo "no matrix size was given, defaults to 1*1"
		OMP_NUM_THREADS=$N_THREADS ./a.out 1 1 1 1;
	else
		OMP_NUM_THREADS=$N_THREADS ./a.out $2 $3 $4 $5;
fi
