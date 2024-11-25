add_library(game_board_for_concepts game_board_for_concepts.cpp)

target_link_libraries(
    game_board_for_concepts PUBLIC
    move_calculator
    minimax_evaluator
    piece_moves
    utility_functs)