
include_directories(
    ${PROJECT_SOURCE_DIR}/board_components
    ${PROJECT_SOURCE_DIR}/board_utilities
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/piece_points
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/move_selector
    ${PROJECT_SOURCE_DIR}/nlohmann_json
    ${PROJECT_SOURCE_DIR}/utility_functs
)

add_executable(
    move_calculator_test
    move_calculator_test.cpp)
target_link_libraries(
    move_calculator_test PRIVATE
    common
    game_board
    move_calculator
    nlohmann_json
    utility_functs
    piece_points
    GTest::GTest
    GTest::Main)

add_executable(game_board_test game_board_test.cpp)
target_link_libraries(
    game_board_test PRIVATE
    game_board
    move_calculator
    utility_functs
    nlohmann_json
    GTest::GTest
    GTest::Main)

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time PRIVATE
    game_board
    move_calculator
    utility_functs)

add_executable(piece_points_test piece_points_test.cpp)
target_link_libraries(
    piece_points_test PRIVATE
    piece_points
    utility_functs
    GTest::GTest
    GTest::Main)

add_executable(minimax_evaluator_test minimax_evaluator_test.cpp)
target_link_libraries(
    minimax_evaluator_test PRIVATE
    minimax_evaluator
    piece_points
    game_board
    utility_functs
    GTest::GTest
    GTest::Main)

add_executable(move_selector_test move_selector_test.cpp)
target_link_libraries(
    move_selector_test PRIVATE
    move_selector
    game_board
    minimax_evaluator
    move_calculator
    piece_points
    utility_functs
    GTest::GTest
    GTest::Main)
