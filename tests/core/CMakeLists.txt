enable_testing()

add_executable(piece_moves_test piece_moves_test.cpp)
target_link_libraries(
    piece_moves_test PRIVATE
    piece_moves
    GTest::gtest_main
)

add_executable(piece_position_points_test piece_position_points_test.cpp)
target_link_libraries(
    piece_position_points_test PRIVATE
    piece_position_points
    utility_functs
    GTest::gtest_main)
target_include_directories(
    piece_position_points_test PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_bpo
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators)

add_executable(
    move_calculator_test
    move_calculator_test.cpp)
target_link_libraries(
    move_calculator_test PRIVATE
    game_board
    move_calculator
    utility_functs
    piece_position_points
    piece_moves
    GTest::gtest_main)

add_executable(game_board_test game_board_test.cpp)
target_link_libraries(
    game_board_test PRIVATE
    game_board
    move_calculator
    piece_moves
    utility_functs
    GTest::gtest_main)

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time PRIVATE
    game_board
    move_calculator
    piece_moves
    utility_functs
    )

add_executable(piece_points_bpo_test piece_points_bpo_test.cpp)
target_link_libraries(
    piece_points_bpo_test PRIVATE
    piece_points_bpo
    utility_functs
    GTest::gtest_main)

add_executable(minimax_evaluator_test minimax_evaluator_test.cpp)
target_link_libraries(
    minimax_evaluator_test PRIVATE
    minimax_evaluator
    piece_moves
    piece_position_points
    game_board
    utility_functs
    GTest::gtest_main)

add_executable(minimax_zobrist_component_test minimax_zobrist_component_test.cpp)
target_link_libraries(
    minimax_zobrist_component_test PRIVATE
    minimax_evaluator
    piece_moves
    piece_position_points
    game_board
    utility_functs
    GTest::gtest_main)

add_executable(
    key_generator_test
    key_generator_test.cpp)
target_link_libraries(
    key_generator_test PRIVATE
    GTest::gtest_main)

add_executable(
        hash_calculator_test
        hash_calculator_test.cpp)
target_link_libraries(
        hash_calculator_test PRIVATE
        hash_calculator
        utility_functs
        game_board
        GTest::gtest_main)

add_executable(
    zobrist_tracker_test
    zobrist_tracker_test.cpp)
target_link_libraries(
    zobrist_tracker_test PRIVATE
    utility_functs
    game_board
    GTest::gtest_main)



add_executable(
    json_utility_test json_utility_test.cpp
)
target_link_libraries(
    json_utility_test PRIVATE
    json_utility_nlohmann
    nlohmann_json::nlohmann_json
    piece_points_bpo
    utility_functs
    GTest::gtest_main
)

add_executable(
    zobrist_calculator_test zobrist_calculator_test.cpp
)
target_link_libraries(
    zobrist_calculator_test PRIVATE hash_calculator
    utility_functs
    game_board
    GTest::gtest_main
)

add_executable(
    zobrist_component_test zobrist_component_test.cpp
)
target_link_libraries(
    zobrist_component_test PRIVATE hash_calculator
    utility_functs
    game_board
    GTest::gtest_main
)

add_executable(
    transposition_table_test transposition_table_test.cpp
)
target_link_libraries(
    transposition_table_test PRIVATE
    hash_calculator
    utility_functs
    game_board
    GTest::gtest_main
)



