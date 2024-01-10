cmake_minimum_required(VERSION 3.26)
project(sequential_impl C)

set(CMAKE_C_STANDARD 11)

add_executable(sequential_impl
        main.c
        sudoku_grid.h
        sudoku_grid.c
        constraint_propagation.c)
