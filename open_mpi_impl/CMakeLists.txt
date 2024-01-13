cmake_minimum_required(VERSION 3.26)
project(open_mpi_impl C)

set(CMAKE_C_STANDARD 11)

add_executable(open_mpi_impl main.c
        constraint_propagation.c
        sudoku_grid.c)


# Find MPI package
find_package(MPI REQUIRED)

# Set include directories and compile flags
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
set(CMAKE_C_COMPILER mpicc)  # Assuming you use the mpicc compiler

# Link with MPI libraries
target_link_libraries(open_mpi_impl ${MPI_C_LIBRARIES})

# Add optimization flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")  # O3 for high-level optimization
