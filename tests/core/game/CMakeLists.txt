add_executable(game_test game_test.cpp)

target_link_libraries(
    game_test PRIVATE
    GTest::gtest_main
    game
    nlohmann_json
    utilities
    gameboard
    moveselection
    terminalout
    )