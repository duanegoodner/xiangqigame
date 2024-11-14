
add_library(
    game_board game_board.cpp)

target_link_libraries(
    game_board PUBLIC
    move_calculator
    minimax_evaluator
    # nlohmann_json
    piece_moves
    utility_functs)

