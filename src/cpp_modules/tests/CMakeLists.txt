# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

include_directories(
    ${PROJECT_SOURCE_DIR}/board_components
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/piece_points
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/move_selector
)

add_executable(
    move_calculator_test
    move_calculator_test.cpp
    # ${PROJECT_SOURCE_DIR}/move_calculator/move_calculator.hpp
    )
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
    move_calculator)

add_executable(piece_points_test piece_points_test.cpp)
target_link_libraries(
    piece_points_test LINK_PUBLIC
    piece_points
    GTest::GTest
    GTest::Main)

add_executable(minimax_evaluator_test minimax_evaluator_test.cpp)
target_link_libraries(
    minimax_evaluator_test LINK_PUBLIC
    minimax_evaluator
    piece_points
    game_board
    GTest::GTest
    GTest::Main)

add_executable(move_selector_test move_selector_test.cpp)
    target_link_libraries(
        move_selector_test LINK_PUBLIC
        move_selector
        game_board
        minimax_evaluator
        move_calculator
        piece_points
        GTest::GTest
        GTest::Main)

# add_executable(minimax_evaluator_3_test minimax_evaluator_3_test.cpp)
# target_link_libraries(
#     minimax_evaluator_3_test LINK_PUBLIC
#     minimax_evaluator_3
#     piece_points
#     game_board
#     GTest::GTest
#     GTest::Main)

# add_executable(move_selector_3_test move_selector_3_test.cpp)
# target_link_libraries(
#     move_selector_3_test LINK_PUBLIC
#     move_selector_3
#     game_board
#     minimax_evaluator
#     move_calculator
#     piece_points
#     GTest::GTest
#     GTest::Main)