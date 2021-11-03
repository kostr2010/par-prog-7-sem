#!/bin/bash

N_THREADS=1
T=1
FILE=""

if [[ -z "$1" ]]
	then
		echo "no number of threads was given. defaults to 1"
	else
		N_THREADS=$1;
fi

if [[ -z "$2" ]]
	then
		echo "no type of input was given. defaults to 1"
	else
		T=$2;
fi

if [[ -z "$3" && $2 == 0 ]]
	then
		echo "no file was given. exiting"
		exit 1
	else
		FILE=$3;

fi

OMP_NUM_THREADS=$N_THREADS ./a.out $T $FILE;