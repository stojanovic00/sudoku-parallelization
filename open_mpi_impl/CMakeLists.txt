cmake_minimum_required(VERSION 3.26)
project(open_mpi_impl C)

set(CMAKE_C_STANDARD 11)

add_executable(open_mpi_impl main.c
        constraint_propagation.c
        sudoku_grid.c)


# Add optimization flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")  # O3 for high-level optimization
