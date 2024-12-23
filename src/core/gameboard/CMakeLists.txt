
add_library(
    gameboard gameboard.cpp game_board_for_concepts.cpp move_calculator.cpp piece_moves.cpp)

target_link_libraries(
    gameboard PUBLIC
    # move_calculator
    nlohmann_json
    moveselection
    # piece_moves
    utilities)

