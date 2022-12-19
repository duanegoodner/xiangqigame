# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

add_library(
    move_calculator
    move_calculator.cpp)

target_include_directories(
    move_calculator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(
move_calculator PRIVATE
${PROJECT_SOURCE_DIR}/board_components
${PROJECT_SOURCE_DIR}/game_board)
