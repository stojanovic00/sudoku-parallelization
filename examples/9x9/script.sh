#!/bin/bash

echo Sequential time:
(time ./script-seq.sh) 2>&1 | awk '/real/ {print $2}'

echo -e "\nMP time:"
(time ./script-mp.sh) 2>&1 | awk '/real/ {print $2}'
