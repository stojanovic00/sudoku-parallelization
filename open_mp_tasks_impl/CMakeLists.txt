cmake_minimum_required(VERSION 3.26)
project(open_mp_tasks_impl C)

set(CMAKE_C_STANDARD 11)

add_executable(open_mp_tasks_impl main.c
        sudoku_grid.c
        constraint_propagation.c)

# Enable OpenMP support
find_package(OpenMP REQUIRED)
target_compile_options(open_mp_tasks_impl PRIVATE ${OpenMP_C_FLAGS})
target_link_libraries(open_mp_tasks_impl PRIVATE ${OpenMP_C_LIBRARIES})

# Add optimization flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")  # O3 for high-level optimization
