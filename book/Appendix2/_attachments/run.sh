#!/bin/bash

# this script performs a number of timed runs of a 
# program given as a required command line argument
# the number of runs is an optional cmdln arg

# script variable N for the number of runs
# assigning N default value of 5 
N=5

# get command line arguments
#  1: name of program is required
#  2: number of runs is optional 
# $@ is the array of command line args (like argv)
# $# is the nunber of command line args (like argc)

# get the program executable name
# note: space between "[ $" and between "0 ]" is important!
if [[ $# -eq 0 ]]
then
  echo "Error, usage: ./run.sh ./a.out [num times]"
  exit 1
else
  PROG=$1
fi

# if they gave optional cmdln arg, set N to it
if [[ $# -gt 1 ]] 
then
  N=$2
fi



# often useful to output some info about what is run:
echo "running $PROG $N times: "
date
echo "======================= "

# do N timed runs of PROG:
for((n=1; n <= $N; n++))
do
echo " "
echo "run $n:"
time $PROG
done
