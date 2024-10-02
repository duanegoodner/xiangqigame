
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
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators
    )
