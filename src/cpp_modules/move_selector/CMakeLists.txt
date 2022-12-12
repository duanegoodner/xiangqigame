# set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -O3)

add_library(
    move_selector
    move_selector.cpp)

    target_include_directories(
        move_selector PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/game_board
        ${PROJECT_SOURCE_DIR}/minimax_evaluator
        ${PROJECT_SOURCE_DIR}/move_calculator
        ${PROJECT_SOURCE_DIR}/piece_points_builder
    )