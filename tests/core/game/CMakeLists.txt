add_executable(game_test game_test.cpp)

target_link_libraries(
    game_test PRIVATE
    GTest::gtest_main
    game
    # game_factory
    nlohmann_json
    utility_functs
    gameboard
    move_evaluator_human_for_concepts
    move_evaluator_minimax_for_concepts
    move_evaluator_random_for_concepts
    piece_position_points_for_concepts
    terminal_output
    )