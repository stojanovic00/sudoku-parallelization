#!/bin/bash
perf stat -r 1 -e cycles -B --log-fd 1 perf bench cmake-build-debug/open_mp_impl ../examples/sudoku_4.csv ../examples/solution_4.csv 2>&1 | awk '/elapsed/ {print $1}'
#perf stat -r 1 -e cycles -B --log-fd 1 perf bench ./test.sh 2>&1 | awk '/elapsed/ {print $1}'

