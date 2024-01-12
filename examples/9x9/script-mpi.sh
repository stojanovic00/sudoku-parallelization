#!/bin/bash


for x in {1..100}
do
  ../../open_mpi_impl/cmake-build-debug/open_mpi_impl sudoku_5.csv solution_5.csv > /dev/null
  #./validate.py solution_5.csv
done
