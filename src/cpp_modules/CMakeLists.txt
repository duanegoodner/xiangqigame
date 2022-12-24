cmake_minimum_required(VERSION 3.19)

project(xiangqi_cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
# set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -O3)

find_package(GTest REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# add_subdirectory(pybind11)


add_subdirectory(board_components)
add_subdirectory(board_utilities)
add_subdirectory(piece_points)
add_subdirectory(game_board)
add_subdirectory(move_calculator)
add_subdirectory(minimax_evaluator)
add_subdirectory(move_selector)
add_subdirectory(tests)
add_subdirectory(pybind_modules)
add_subdirectory(utility_functs)


   







