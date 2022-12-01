add_executable(move_calculator_test move_calculator_test.cpp)
target_link_libraries(
    move_calculator_test LINK_PUBLIC
    move_calculator
    GTest::GTest
    GTest::Main)

add_executable(game_board_test game_board_test.cpp)
target_link_libraries(
    game_board_test LINK_PUBLIC
    game_board
    move_calculator
    GTest::GTest
    GTest::Main)

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time LINK_PUBLIC
    game_board
    move_calculator
)

set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra)