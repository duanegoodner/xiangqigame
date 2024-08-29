cmake_minimum_required(VERSION 3.21)

project(
        xiangqi_cpp
        VERSION 0.2
        DESCRIPTION "GameBoard and MoveSelector for xiangqigame"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

# enable gprof
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# normally control these via IDE (so comment out for now)
# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

SET(CMAKE_UTIL_DIR ${PROJECT_SOURCE_DIR}/cmake)
SET(SOURCE_CODE_DIR ${PROJECT_SOURCE_DIR}/src/core)
SET(DATA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/data)
SET(DATA_DESTINATION_DIR ${CMAKE_BINARY_DIR}/data)


# Create a configuration header file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
    "${CMAKE_BINARY_DIR}/config.hpp"
    @ONLY
)
include_directories("${CMAKE_BINARY_DIR}")

include("${CMAKE_UTIL_DIR}/fetch_packages.cmake")
include("${CMAKE_UTIL_DIR}/add_subdirectories.cmake")

# Find Python 3 interpreter
find_package(Python REQUIRED COMPONENTS Interpreter)

# Display the path to the Python site-packages dir
message(STATUS "Python site-packages dir: ${Python_SITELIB}")

if(SKBUILD EQUAL 1)
  message(STATUS "CMake is being run by scikit-build")
elseif(SKBUILD EQUAL 2)
  message(STATUS "CMake is being run by scikit-build-core")
  else()
message(STATUS "CMake is not being run by scikit-build nor by scikt-build-core")
  endif()
