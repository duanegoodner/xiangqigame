add_executable(
    terminal_output_test
    terminal_output_test.cpp
    )

target_link_libraries(
    terminal_output_test PRIVATE
    GTest::gtest_main
    game_board_for_concepts
    terminal_output
    )