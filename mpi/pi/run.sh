#!/bin/bash
# Run this script with: bsub -Whh:mm < run.sh
# The -W option sets a wallclock timelimit

#BSUB -o ./%J.o
#BSUB -a mpich_gm
#BSUB -q normal
#BSUB -J pi
#BSUB -n 10

# NOTE: don't delete echo statement
echo

mpirun.lsf -np 10 ./pi

