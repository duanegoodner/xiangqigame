add_library(game_factory game_factory.cpp)
target_link_libraries(
    game_factory
    PUBLIC
    GTest::gtest_main
    game
    nlohmann_json
    utility_functs
    game_board_for_concepts
    move_evaluator_human_for_concepts
    move_evaluator_minimax_for_concepts
    move_evaluator_random_for_concepts
    piece_position_points_for_concepts
    )