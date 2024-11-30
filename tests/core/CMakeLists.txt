enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


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
    nlohmann_json
    utility_functs
    GTest::gtest_main)

add_executable(piece_position_points_concept_test piece_position_points_concept_test.cpp)
target_link_libraries(
    piece_position_points_concept_test PRIVATE
    builders
    piece_position_points_for_concepts
    nlohmann_json
    utility_functs
    GTest::gtest_main)

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

add_executable(game_board_concept_test game_board_concept_test.cpp)
target_link_libraries(
    game_board_concept_test PRIVATE
    builders
    game_board_for_concepts
    move_calculator
    piece_moves
    utility_functs
    GTest::gtest_main)

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time PRIVATE
    game_board
    nlohmann_json
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

add_executable(minimax_zobrist_component_test minimax_zobrist_component_test.cpp)
target_link_libraries(
    minimax_zobrist_component_test PRIVATE
    minimax_evaluator
    piece_moves
    piece_position_points
    game_board
    utility_functs
    GTest::gtest_main)

# add_executable(minimax_concept_test minimax_concept_test.cpp)
# target_link_libraries(
#     minimax_concept_test PRIVATE
#     builders
#     minimax_evaluator
#     piece_moves
#     piece_position_points_for_concepts
#     game_board_for_concepts
#     utility_functs
#     GTest::gtest_main)

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
    json_utility_nlohmann
    minimax_evaluator
    piece_moves
    piece_position_points
    game_board
    GTest::gtest_main)

add_executable(
    zobrist_calculator_concept_test
    zobrist_calculator_concept_test.cpp)
target_link_libraries(
    zobrist_calculator_concept_test PRIVATE
    nlohmann_json
    utility_functs
    game_board
    GTest::gtest_main)

add_executable(
    zobrist_component_concept_test
    zobrist_component_concept_test.cpp)
target_link_libraries(
    zobrist_component_concept_test PRIVATE
    nlohmann_json
    utility_functs
    game_board
    GTest::gtest_main)



add_executable(
    zobrist_coordinator_test
    zobrist_coordinator_test.cpp)
target_link_libraries(
    zobrist_coordinator_test PRIVATE
    utility_functs
    game_board
    GTest::gtest_main)

add_executable(
    zobrist_coordinator_concept_test
    zobrist_coordinator_concept_test.cpp)
target_link_libraries(
    zobrist_coordinator_concept_test PRIVATE
    nlohmann_json
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
    zobrist_calculator_test PRIVATE
    utility_functs
    game_board
    nlohmann_json
    GTest::gtest_main
)

add_executable(
    zobrist_component_test zobrist_component_test.cpp
)
target_link_libraries(
    zobrist_component_test PRIVATE 
    utility_functs
    game_board
    GTest::gtest_main
)

add_executable(
    transposition_table_test transposition_table_test.cpp
)
target_link_libraries(
    transposition_table_test PRIVATE
    utility_functs
    game_board
    GTest::gtest_main
)



