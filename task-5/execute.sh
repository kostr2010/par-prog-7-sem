#!/bin/bash

N_THREADS=1
N_ITERATIONS=1
N_SERIES=0

if [[ -z "$1" ]]
	then
		echo "no number of threads was given. defaults to 1"
	else
		N_THREADS=$1;
fi

if [[ -z "$2" ]]
	then
		echo "no number of iterations was given. defaults to 1"
	else
		N_ITERATIONS=$2;
fi

if [[ -z "$3" ]]
	then
		echo "no series were selected. defaulted to exponent"
	else
		N_SERIES=$3;
fi

if [[ -z "$4" ]]
	then
		echo "no point was specified. defaulted to 1.0"
		OMP_NUM_THREADS=$N_THREADS ./a.out $N_ITERATIONS $N_SERIES 1.0;
	else
		OMP_NUM_THREADS=$N_THREADS ./a.out $N_ITERATIONS $N_SERIES $4;
fi
