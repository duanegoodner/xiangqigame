add_library(
    move_selector
    move_selector.cpp)

target_link_libraries(
    move_selector PUBLIC
    board_components
    board_utilities
    game_board
    minimax_evaluator
    move_calculator
    piece_points
    utility_functs
)

target_include_directories(
    move_selector PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/game_board
    # ${PROJECT_SOURCE_DIR}/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/piece_points
    # ${PROJECT_SOURCE_DIR}/utility_functs
)
