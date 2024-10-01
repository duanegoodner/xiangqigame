
add_library(
    game_board game_board.cpp)

target_link_libraries(
    game_board PRIVATE
    move_calculator
    minimax_evaluator
    piece_moves
    nlohmann_json::nlohmann_json
    hash_calculator
    utility_functs)

target_include_directories(
    game_board PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/move_calculator
    ${PROJECT_SOURCE_DIR}/src/core/hash_calculator
    ${PROJECT_SOURCE_DIR}/src/core/piece_points
    ${PROJECT_SOURCE_DIR}/src/core/piece_moves
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators
    )
