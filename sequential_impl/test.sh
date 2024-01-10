#!/bin/bash


for n in {0..4}
do
  cmake-build-debug/sequential_impl ../examples/sudoku_$n.csv ../examples/solution_$n.csv > /dev/null
done




for n in {0..4}
  do
  if ../examples/check ../examples/solution_$n.csv ../examples/test_$n.csv > /dev/null; then
      echo "Test $n: passed"
  else
      echo "Test $n: failed"
  fi
done


