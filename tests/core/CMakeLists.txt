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
    nlohmann_json::nlohmann_json
    GTest::gtest_main)
target_include_directories(
    piece_position_points_test PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators
    ${rapidjson_SOURCE_DIR}/include)

add_executable(
    move_calculator_test
    move_calculator_test.cpp)
target_link_libraries(
    move_calculator_test PRIVATE
    game_board
    move_calculator
    nlohmann_json::nlohmann_json
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
    nlohmann_json::nlohmann_json
    GTest::gtest_main)

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time PRIVATE
    game_board
    move_calculator
    piece_moves
    utility_functs
    nlohmann_json::nlohmann_json)

add_executable(piece_points_spec_test piece_points_spec_test.cpp)
target_link_libraries(
    piece_points_spec_test PRIVATE
    piece_points_spec
    utility_functs
    nlohmann_json::nlohmann_json
    GTest::gtest_main)
target_include_directories(
    piece_points_spec_test PRIVATE
    # ${PROJECT_SOURCE_DIR}/src/core/json_internal
    ${rapidjson_SOURCE_DIR}/include
    )

add_executable(minimax_evaluator_test minimax_evaluator_test.cpp)
target_link_libraries(
    minimax_evaluator_test PRIVATE
    minimax_evaluator
    piece_moves
    piece_position_points
    game_board
    # json_internal
    utility_functs
    # json_internal
    nlohmann_json::nlohmann_json
    GTest::gtest_main)
target_include_directories(
    minimax_evaluator_test PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_spec
    ${rapidjson_SOURCE_DIR}/include)

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
        nlohmann_json::nlohmann_json
        utility_functs
        game_board
        GTest::gtest_main)

# add_executable(
#     json_internal_test
#     json_internal_test.cpp)
# target_link_libraries(
#     json_internal_test PRIVATE
#     json_internal
#     utility_functs
#     GTest::gtest_main)
# target_include_directories(
#     json_internal_test PRIVATE
#     ${PROJECT_SOURCE_DIR}/src/core/json_internal
# )

# add_executable(json_io_test json_io_test.cpp)
# target_link_libraries(
#     json_io_test PRIVATE
#     GTest::gtest_main
#     json_interface
#     json_io
#     utility_functs)
