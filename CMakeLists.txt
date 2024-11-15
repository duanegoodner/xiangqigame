cmake_minimum_required(VERSION 3.21)
message(STATUS "Using CMake version ${CMAKE_VERSION}")

project(xiangqigame LANGUAGES CXX VERSION 1.0.0)
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "Project version: ${PROJECT_VERSION}")

message(STATUS "SKBUILD = ${SKBUILD}")
message(STATUS "SKBUILD_STATE = ${SKBUILD_STATE}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_PROTOTYPES "Build prototype executables" OFF)
option(BUILD_TESTS "Build core tests" OFF)
option(ENABLE_PROFILING "Enable gprof" OFF)

if (ENABLE_PROFILING)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()

SET(SOURCE_CODE_DIR ${PROJECT_SOURCE_DIR}/src/core)
SET(DATA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/data)
SET(CMAKE_UTIL_DIR ${PROJECT_SOURCE_DIR}/cmake)

# fetch packages early b/c have problems if fetch Pybind11 before finding Python
include("${CMAKE_UTIL_DIR}/fetch_packages.cmake")

# find python now, so we can use it for reading toml
find_package(Python REQUIRED COMPONENTS Interpreter)
message(STATUS "Python executable: ${Python_EXECUTABLE}")
message(STATUS "Python site-packages dir: ${Python_SITELIB}")

# Eead project name/version/description from pyproject.toml.
# For now, just informational to compare with values set in CMAKE
include("${CMAKE_UTIL_DIR}/read_pyproject_toml.cmake")

# if (NOT SKBUILD)
#   set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME})
# endif()  


if (SKBUILD)
  if(SKBUILD_STATE STREQUAL "editable")
    set(DATA_DESTINATION_DIR ${DATA_SOURCE_DIR})
  else()
    set(DATA_DESTINATION_DIR "${Python_SITELIB}/xiangqipy/data")
  endif()
else()
  SET(DATA_DESTINATION_DIR ${CMAKE_BINARY_DIR}/data)
endif()
message(STATUS "DATA_DESTINATION_DIR = ${DATA_DESTINATION_DIR}")

message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "DATA_SOURCE_DIR = ${DATA_SOURCE_DIR}")

# Create a configuration header file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
    "${CMAKE_BINARY_DIR}/config.hpp"
    @ONLY
)
include_directories("${CMAKE_BINARY_DIR}")
message(STATUS "config.hpp path: ${CMAKE_BINARY_DIR}/config.hpp")

include_directories(${PROJECT_SOURCE_DIR}/include)
include("${CMAKE_UTIL_DIR}/add_subdirectories.cmake")

