cmake_minimum_required(VERSION 3.15)

project(xiangqi_cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -O3)

find_package(GTest REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# add_subdirectory(pybind11)

add_subdirectory(game_board)
add_subdirectory(move_calculator)
add_subdirectory(piece_points)
add_subdirectory(tests)
add_subdirectory(game_board_py)


   







