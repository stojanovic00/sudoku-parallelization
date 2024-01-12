#!/bin/bash


for x in {1..100}
do
  ../../sequential_impl/cmake-build-debug/sequential_impl sudoku_5.csv solution_5.csv > /dev/null
  #./validate.py solution_5.csv
done


