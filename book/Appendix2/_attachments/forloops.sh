#!/bin/bash

echo "for loop over set of values" 
# interate over a set of given values
# repeats once for each element in the list
for i in cat dog bunny 
  do
    echo "next animal is: $i"
  done

echo 

echo "for loop over sequence" 
# {1..5} is the set of values in the sequence: 1,2,3,4,5
# this iterates 5 times 
# each time i is `infilei` for i: 1, 2,3,4,5
for i in infile{1..5}
  do
    echo "next file is: $i"
  done

echo 

echo "for loop over set created with ls" 
# this iterates over all the files in a subdirectory named input
# $(ls input/): creates a set of value that are all the file
# and directory names in the input/ subdirectory 
for i in $(ls input/)
  do
    echo "next input/ file is: $i"
  done
