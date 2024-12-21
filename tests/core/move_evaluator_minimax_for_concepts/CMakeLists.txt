add_executable(move_evaluator_minimax_for_concepts_test move_evaluator_minimax_for_concepts_test.cpp)
target_link_libraries(
    move_evaluator_minimax_for_concepts_test PRIVATE
    moveselection
    piece_position_points_for_concepts
    gameboard
    GTest::gtest_main)