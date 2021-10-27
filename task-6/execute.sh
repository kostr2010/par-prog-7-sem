#!/bin/bash

N_THREADS=1
N=1
N_ALGORITHM=0

if [[ -z "$1" ]]
	then
		echo "no number of threads was given. defaults to 1"
	else
		N_THREADS=$1;
fi

if [[ -z "$2" ]]
	then
		echo "no number was given. defaults to 1"
	else
		N=$2;
fi

if [[ -z "$3" ]]
	then
		echo "no algorithm was selected. defaulted to Eratosphenes"
	else
		N_ALGORITHM=$3;
fi

OMP_NUM_THREADS=$N_THREADS ./a.out $N $N_ALGORITHM;