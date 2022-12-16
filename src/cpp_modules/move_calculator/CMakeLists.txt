# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

add_library(
    move_calculator
    move_calculator.cpp
    board_utilities.cpp)

find_package(OpenMP)

target_link_libraries(
    move_calculator PUBLIC
    game_board
    OpenMP::OpenMP_CXX
)

    target_include_directories(
    move_calculator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/board_components
    ${PROJECT_SOURCE_DIR}/game_board
    )