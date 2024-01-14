#!/bin/bash

# Number of iterations
iterations=300

# Initialize sum
sum=0

# Run the command 300 times
for ((i = 1; i <= iterations; i++)); do
    result=$(perf stat --log-fd 1 ../open_mp_impl/cmake-build-debug/open_mp_impl ../examples/9x9/sudoku_5.csv ../examples/9x9/solution_5.csv | awk '/time elapsed/ {print $1}')

    # Add $1 to the sum
    sum=$(awk "BEGIN {print $sum + $result}")
done

# Calculate the average
average=$(awk "BEGIN {print $sum / $iterations}")

echo "[MP] Average time elapsed over $iterations runs: $average seconds"

