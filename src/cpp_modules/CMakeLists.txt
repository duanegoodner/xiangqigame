cmake_minimum_required(VERSION 3.19)

project(xiangqi_cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)
        
# find_package(GTest REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/ec25eea8f8237cf86c30703f59747e42f34b6f75.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(nlohmann_json REQUIRED)
find_package(nlohmann_json_schema_validator CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)


add_subdirectory(src/board_components)
add_subdirectory(src/board_utilities)
add_subdirectory(src/common)
add_subdirectory(src/piece_points)
add_subdirectory(src/piece_points_spec)
add_subdirectory(src/game_board)
add_subdirectory(src/move_calculator)
add_subdirectory(src/minimax_evaluator)
add_subdirectory(src/move_selector)
add_subdirectory(tests)
add_subdirectory(src/pybind_modules)
add_subdirectory(src/utility_functs)
add_subdirectory(src/hash_calculator)
add_subdirectory(src/json_validation)
# add_subdirectory(nlohmann_json)

include_directories(
        ${PROJECT_SOURCE_DIR}/src/board_components
        ${PROJECT_SOURCE_DIR}/src/board_utilities
        ${PROJECT_SOURCE_DIR}/src/move_calculator
        ${PROJECT_SOURCE_DIR}/src/game_board
        ${PROJECT_SOURCE_DIR}/src/piece_points
        ${PROJECT_SOURCE_DIR}/src/minimax_evaluator
        ${PROJECT_SOURCE_DIR}/src/move_selector
)


   







