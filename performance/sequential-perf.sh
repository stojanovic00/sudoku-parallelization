#!/bin/bash

# Number of iterations
iterations=300

# Initialize sum
sum=0

# Run the command N times
for ((i = 1; i <= iterations; i++)); do
    result=$(perf stat --log-fd 1 ../sequential_impl/cmake-build-debug/sequential_impl ../examples/9x9/sudoku_5.csv ../examples/9x9/solution_5.csv | awk '/time elapsed/ {print $1}')

    # Add $1 to the sum
    sum=$(awk "BEGIN {print $sum + $result}")
done

# Calculate the average
average=$(awk "BEGIN {print $sum / $iterations}")

echo "[Sequential] Average time elapsed over $iterations runs: $average seconds"

