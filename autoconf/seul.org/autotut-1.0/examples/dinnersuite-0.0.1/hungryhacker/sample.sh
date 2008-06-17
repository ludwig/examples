#!/bin/sh

if [ ! -x ./hungryhacker ]; then
    echo "hungryhacker must be compiled for this"
    echo "script to work. No need to install, this"
    echo "script will run it from this directory."
    exit 1;
fi

./hungryhacker -n alan   -s 12 -d 11 -a 10 &
./hungryhacker -n larry  -s 9  -d 9  -a 11 &
./hungryhacker -n riel   -s 8  -d 11 -a 9  &
./hungryhacker -n xach   -s 11 -d 10 -a 12 & 
./hungryhacker -n kojima -s 9  -d 14 -a 5  &
./hungryhacker -n raph   -s 11 -d 11 -a 11 &
./hungryhacker -n rgooch -s 10 -d 10 -a 11 &

