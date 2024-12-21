add_library(game game.cpp game_factory.cpp)
target_link_libraries(
    game PUBLIC
    gameboard
    nlohmann_json
    utility_functs
    gameboard
    move_evaluator_human_for_concepts
    move_evaluator_minimax_for_concepts
    move_evaluator_random_for_concepts
    piece_position_points_for_concepts
    )