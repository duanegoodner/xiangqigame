add_executable(player_spec_builder_test player_spec_builder_test.cpp)
target_link_libraries(
    player_spec_builder_test PRIVATE
    GTest::gtest_main
    jsonio
    utilities
)

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

add_executable(game_runner_test game_runner_test.cpp)
target_link_libraries(
    game_runner_test PRIVATE
    GTest::gtest_main
    game
    jsonio
    terminalout
    utilities
    )