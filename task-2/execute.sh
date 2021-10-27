#!/bin/bash

N_THREADS=1

if [[ -z "$1" ]]
	then
		echo "no number of threads was given. defaults to 1"
	else
		N_THREADS=$1;
fi

if [[ -z "$2" ]]
	then
		echo "no number was given, defaults to 10"
		OMP_NUM_THREADS=$N_THREADS ./a.out 10;
	else
		OMP_NUM_THREADS=$N_THREADS ./a.out $2;
fi
