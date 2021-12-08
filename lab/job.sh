#!/bin/bash

#PBS -l walltime=00:01:00,nodes=2:ppn=2
#PBS -N my_job
#PBS -q batch

cd $PBS_O_WORKDIR
time mpirun --hostfile $PBS_NODEFILE -np 4 ./a.out

