
include_directories(
    ${PROJECT_SOURCE_DIR}/board_components
    ${PROJECT_SOURCE_DIR}/board_utilities
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/piece_points
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/move_selector
)

add_executable(
    move_calculator_test
    move_calculator_test.cpp)
target_link_libraries(
    move_calculator_test PUBLIC
    move_calculator
    GTest::GTest
    GTest::Main)

add_executable(game_board_test game_board_test.cpp ../zobrist_hash/zobrist_hash.hpp)
target_link_libraries(
    game_board_test PUBLIC
    game_board
    move_calculator
    GTest::GTest
    GTest::Main)

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time PUBLIC
    game_board
    move_calculator)

add_executable(piece_points_test piece_points_test.cpp)
target_link_libraries(
    piece_points_test PUBLIC
    piece_points
    GTest::GTest
    GTest::Main)

add_executable(minimax_evaluator_test minimax_evaluator_test.cpp)
target_link_libraries(
    minimax_evaluator_test PUBLIC
    minimax_evaluator
    piece_points
    game_board
    GTest::GTest
    GTest::Main)

add_executable(move_selector_test move_selector_test.cpp)
target_link_libraries(
    move_selector_test PUBLIC
    move_selector
    game_board
    minimax_evaluator
    move_calculator
    piece_points
    utility_functs
    GTest::GTest
    GTest::Main)
