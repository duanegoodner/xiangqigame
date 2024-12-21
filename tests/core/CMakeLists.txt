enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(game)
add_subdirectory(move_evaluator_random_for_concepts)
add_subdirectory(move_evaluator_minimax_for_concepts)
add_subdirectory(move_evaluator_human_for_concepts)
add_subdirectory(move_translator)
add_subdirectory(terminal_output)

add_executable(piece_moves_test piece_moves_test.cpp)
target_link_libraries(
    piece_moves_test PRIVATE
    gameboard
    GTest::gtest_main
)

add_executable(piece_position_points_test piece_position_points_test.cpp)
target_link_libraries(
    piece_position_points_test PRIVATE
    piecepoints
    nlohmann_json
    utilities
    GTest::gtest_main)

add_executable(piece_position_points_concept_test piece_position_points_concept_test.cpp)
target_link_libraries(
    piece_position_points_concept_test PRIVATE
    piecepoints
    nlohmann_json
    utilities
    GTest::gtest_main)

add_executable(
    move_calculator_test
    move_calculator_test.cpp)
target_link_libraries(
    move_calculator_test PRIVATE
    gameboard
    # move_calculator
    utilities
    piecepoints
    # piece_moves
    GTest::gtest_main)

add_executable(game_board_test game_board_test.cpp)
target_link_libraries(
    game_board_test PRIVATE
    gameboard
    # move_calculator
    # piece_moves
    utilities
    GTest::gtest_main)

add_executable(game_board_concept_test game_board_concept_test.cpp)
target_link_libraries(
    game_board_concept_test PRIVATE
    gameboard
    # move_calculator
    # piece_moves
    GTest::gtest_main)

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time PRIVATE
    gameboard
    nlohmann_json
    # move_calculator
    # piece_moves
    utilities
    )

add_executable(piece_points_bpo_test piece_points_bpo_test.cpp)
target_link_libraries(
    piece_points_bpo_test PRIVATE
    piecepoints
    utilities
    GTest::gtest_main)

add_executable(minimax_zobrist_component_test minimax_zobrist_component_test.cpp)
target_link_libraries(
    minimax_zobrist_component_test PRIVATE
    moveselection
    # piece_moves
    piecepoints
    gameboard
    GTest::gtest_main)

add_executable(
    key_generator_test
    key_generator_test.cpp)
target_link_libraries(
    key_generator_test PRIVATE
    GTest::gtest_main)

add_executable(
    transposition_table_pruner_test
    transposition_table_pruner_test.cpp)
target_link_libraries(
    transposition_table_pruner_test PRIVATE
    jsonio
    moveselection
    # piece_moves
    piecepoints
    gameboard
    GTest::gtest_main)

add_executable(
    zobrist_calculator_concept_test
    zobrist_calculator_concept_test.cpp)
target_link_libraries(
    zobrist_calculator_concept_test PRIVATE
    gameboard
    GTest::gtest_main)

add_executable(
    zobrist_component_concept_test
    zobrist_component_concept_test.cpp)
target_link_libraries(
    zobrist_component_concept_test PRIVATE
    gameboard
    GTest::gtest_main)

add_executable(
    zobrist_coordinator_test
    zobrist_coordinator_test.cpp)
target_link_libraries(
    zobrist_coordinator_test PRIVATE
    utilities
    gameboard
    GTest::gtest_main)

add_executable(
    zobrist_coordinator_concept_test
    zobrist_coordinator_concept_test.cpp)
target_link_libraries(
    zobrist_coordinator_concept_test PRIVATE
    gameboard
    GTest::gtest_main)

add_executable(
    json_utility_test json_utility_test.cpp
)
target_link_libraries(
    json_utility_test PRIVATE
    jsonio
    nlohmann_json::nlohmann_json
    piecepoints
    utilities
    GTest::gtest_main
)

add_executable(
    zobrist_calculator_test zobrist_calculator_test.cpp
)
target_link_libraries(
    zobrist_calculator_test PRIVATE
    utilities
    gameboard
    nlohmann_json
    GTest::gtest_main
)

add_executable(
    zobrist_component_test zobrist_component_test.cpp
)
target_link_libraries(
    zobrist_component_test PRIVATE 
    utilities
    gameboard
    GTest::gtest_main
)

add_executable(
    transposition_table_test transposition_table_test.cpp
)
target_link_libraries(
    transposition_table_test PRIVATE
    utilities
    gameboard
    GTest::gtest_main
)



