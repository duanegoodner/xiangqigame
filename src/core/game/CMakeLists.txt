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

add_executable(run_game run_game.cpp)
target_link_libraries(
    run_game PRIVATE
    game
    jsonio
    terminalout
    utilities
)