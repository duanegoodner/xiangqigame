
add_library(
    gameboard gameboard.cpp game_board_for_concepts.cpp move_calculator.cpp)

target_link_libraries(
    gameboard PUBLIC
    # move_calculator
    nlohmann_json
    minimax_evaluator
    piece_moves
    utility_functs)

