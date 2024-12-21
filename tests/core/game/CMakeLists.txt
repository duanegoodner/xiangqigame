add_executable(game_test game_test.cpp)

target_link_libraries(
    game_test PRIVATE
    GTest::gtest_main
    game
    # game_factory
    nlohmann_json
    utility_functs
    gameboard
    moveselection
    terminal_output
    )