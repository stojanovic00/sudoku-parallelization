#!/bin/bash

echo Sequential time:
(time ./script-seq.sh) 2>&1 | awk '/real/ {print $2}'

echo -e "\nMP (primitives) time:"
(time ./script-mp.sh) 2>&1 | awk '/real/ {print $2}'

echo -e "\nMP (tasks) time:"
(time ./script-tsks.sh) 2>&1 | awk '/real/ {print $2}'

echo -e "\nMPI time:"
(time ./script-mpi.sh) 2>&1 | awk '/real/ {print $2}'
