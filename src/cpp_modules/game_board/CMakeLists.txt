
add_library(
    game_board game_board.cpp)

target_link_libraries(
    game_board PRIVATE
    board_components
    board_utilities
    common
    move_calculator
    minimax_evaluator
    move_selector
    nlohmann_json
    hash_calculator
    utility_functs)

target_include_directories(
    game_board PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/hash_calculator
    ${PROJECT_SOURCE_DIR}/move_selector
    ${PROJECT_SOURCE_DIR}/piece_points
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    )
