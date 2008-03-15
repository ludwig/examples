#!/bin/bash

# Run this script with: bsub -Whh:mm < run.sh
# The -W option sets a wallclock timelimit

#BSUB -o ./%J.o
#BSUB -a mpich_gm
#BSUB -q normal
#BSUB -J model20051119-2
#BSUB -n 100

# NOTE: do not delete!
echo

mpirun.lsf -np 100 ./mpihello

