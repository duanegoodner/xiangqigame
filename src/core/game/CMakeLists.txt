add_library(game game.cpp game_factory.cpp)
target_link_libraries(
    game PUBLIC
    game_board_for_concepts
    nlohmann_json
    utility_functs
    game_board_for_concepts
    move_evaluator_human_for_concepts
    move_evaluator_minimax_for_concepts
    move_evaluator_random_for_concepts
    piece_position_points_for_concepts
    )