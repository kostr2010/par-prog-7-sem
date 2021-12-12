#!/bin/bash

#PBS -l walltime=00:10:00,nodes=7:ppn=4
#PBS -N my_job
#PBS -q batch

cd $PBS_O_WORKDIR

for ((i = 1; i <= 32; ++i)); do
	time mpirun --hostfile $PBS_NODEFILE -np $i ./1g-parallel.out
done 


