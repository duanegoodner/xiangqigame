
include_directories(
    ${PROJECT_SOURCE_DIR}/src/board_components
    ${PROJECT_SOURCE_DIR}/src/board_utilities
    ${PROJECT_SOURCE_DIR}/src/common
    ${PROJECT_SOURCE_DIR}/src/move_calculator
    ${PROJECT_SOURCE_DIR}/src/game_board
    ${PROJECT_SOURCE_DIR}/src/piece_points
    ${PROJECT_SOURCE_DIR}/src/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/src/move_selector
    ${PROJECT_SOURCE_DIR}/src/utility_functs
)

enable_testing()

add_executable(
    move_calculator_test
    move_calculator_test.cpp)
target_link_libraries(
    move_calculator_test PRIVATE
    common
    game_board
    move_calculator
    nlohmann_json::nlohmann_json
    utility_functs
    piece_points
    GTest::gtest_main
    # GTest::GTest
    # GTest::Main
    )

add_executable(game_board_test game_board_test.cpp)
target_link_libraries(
    game_board_test PRIVATE
    game_board
    move_calculator
    utility_functs
    nlohmann_json::nlohmann_json
    GTest::gtest_main
    # GTest::GTest
    # GTest::Main
    )

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time PRIVATE
    game_board
    move_calculator
    utility_functs
    nlohmann_json::nlohmann_json)

add_executable(piece_points_test piece_points_test.cpp)
target_link_libraries(
    piece_points_test PRIVATE
    piece_points
    utility_functs
    nlohmann_json::nlohmann_json
    GTest::gtest_main
    # GTest::GTest
    # GTest::Main
    )

add_executable(piece_points_spec_test piece_points_spec_test.cpp)
target_link_libraries(
    piece_points_spec_test PRIVATE
    piece_points_spec
    utility_functs
    nlohmann_json::nlohmann_json
    GTest::gtest_main
    # GTest::GTest
    # GTest::Main
    )

add_executable(minimax_evaluator_test minimax_evaluator_test.cpp)
target_link_libraries(
    minimax_evaluator_test PRIVATE
    minimax_evaluator
    piece_points
    game_board
    utility_functs
    nlohmann_json::nlohmann_json
    GTest::gtest_main
    # GTest::GTest
    # GTest::Main
    )

add_executable(move_selector_test move_selector_test.cpp)
target_link_libraries(
    move_selector_test PRIVATE
    move_selector
    game_board
    minimax_evaluator
    move_calculator
    piece_points
    utility_functs
    nlohmann_json::nlohmann_json
    GTest::gtest_main
    # GTest::GTest
    # GTest::Main
    )

