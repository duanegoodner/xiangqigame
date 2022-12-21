# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

add_library(
    move_selector
    move_selector.cpp)

target_link_libraries(
    move_selector
    game_board
    minimax_evaluator
    move_calculator
    piece_points
)

target_include_directories(
    move_selector PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/board_components
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/piece_points
    ${PROJECT_SOURCE_DIR}/utility_functs
)

add_library(
    move_selector_3
    move_selector_3.cpp
)

target_link_libraries(
    move_selector_3
    game_board
    minimax_evaluator_3
    move_calculator
    piece_points 
)

target_include_directories(
    move_selector_3 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/board_components
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/piece_points
    ${PROJECT_SOURCE_DIR}/utility_functs)
