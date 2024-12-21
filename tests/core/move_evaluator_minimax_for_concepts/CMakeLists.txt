add_executable(move_evaluator_minimax_for_concepts_test move_evaluator_minimax_for_concepts_test.cpp)
target_link_libraries(
    move_evaluator_minimax_for_concepts_test PRIVATE
    moveselection
    piecepoints
    gameboard
    GTest::gtest_main)