add_library(game game.cpp game_factory.cpp)
target_link_libraries(
    game PUBLIC
    gameboard
    nlohmann_json
    utility_functs
    gameboard
    moveselection
    piece_position_points_for_concepts
    )