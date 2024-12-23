add_library(game game.cpp game_factory.cpp)
target_link_libraries(
    game PUBLIC
    gameboard
    nlohmann_json
    utilities
    gameboard
    moveselection
    piecepoints
    )