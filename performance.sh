#!/bin/bash

echo Sequential:

perf stat --log-fd 1 sequential_impl/cmake-build-debug/sequential_impl ./examples/9x9/sudoku_4.csv ./examples/9x9/solution_4.csv | awk '/time elapsed|solved/ {print $1" "$2}'

./examples/9x9/validate.py ./examples/9x9/solution_4.csv | awk '{print $2}'

rm ./examples/9x9/solution_4.csv

echo -e "\nOpenMP primitives:"

perf stat --log-fd 1 open_mp_impl/cmake-build-debug/open_mp_impl ./examples/9x9/sudoku_4.csv ./examples/9x9/solution_4.csv | awk '/time elapsed|solved/ {print $1" "$2}'

./examples/9x9/validate.py ./examples/9x9/solution_4.csv | awk '{print $2}'

rm ./examples/9x9/solution_4.csv

