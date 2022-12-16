# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

add_executable(move_calculator_test move_calculator_test.cpp)
target_link_libraries(
    move_calculator_test LINK_PUBLIC
    move_calculator
    GTest::GTest
    GTest::Main)
# target_include_directories(
#     move_calculator_test LINK_PUBLIC
#     ${PROJECT_SOURCE_DIR}/move_calculator)

add_executable(game_board_test game_board_test.cpp)
target_link_libraries(
    game_board_test LINK_PUBLIC
    game_board
    move_calculator
    GTest::GTest
    GTest::Main)
# target_include_directories(
#     game_board_test LINK_PUBLIC
#     ${PROJECT_SOURCE_DIR}/move_calculator
#     ${PROJECT_SOURCE_DIR}/game_board)

add_executable(move_calc_time move_calc_timer.cpp)
target_link_libraries(
    move_calc_time LINK_PUBLIC
    game_board
    move_calculator)
# target_include_directories(
#     move_calc_time LINK_PUBLIC
#     ${PROJECT_SOURCE_DIR}/move_calculator
#     ${PROJECT_SOURCE_DIR}/game_board)

add_executable(piece_points_builder_test piece_points_builder_test.cpp)
target_link_libraries(
    piece_points_builder_test LINK_PUBLIC
    piece_points_builder
    GTest::GTest
    GTest::Main)
# target_include_directories(
#     pice_points_builder_test LINK_PUBLIC
#     ${PROJECT_SOURCE_DIR}/piece_points_builder)

add_executable(minimax_evaluator_test minimax_evaluator_test.cpp)
target_link_libraries(
    minimax_evaluator_test LINK_PUBLIC
    minimax_evaluator
    piece_points_builder
    game_board
    GTest::GTest
    GTest::Main)
# target_include_directories(
#     minimax_evaluator_test LINK_PUBLIC
#     ${PROJECT_SOURCE_DIR}/board_components
    # ${PROJECT_SOURCE_DIR}/piece_points_builder
    # ${PROJECT_SOURCE_DIR}/game_board
# )

add_executable(move_selector_test move_selector_test.cpp)
target_link_libraries(
    move_selector_test LINK_PUBLIC
    move_selector
    game_board
    minimax_evaluator
    move_calculator
    piece_points_builder
    GTest::GTest
    GTest::Main)
# target_include_directories(
#     move_selector_test LINK_PUBLIC
#     ${PROJECT_SOURCE_DIR}/move_selector
#     ${PROJECT_SOURCE_DIR}/game_board
#     ${PROJECT_SOURCE_DIR}/minimax_evaluator
#     ${PROJECT_SOURCE_DIR}/move_calculator
#     ${PROJECT_SOURCE_DIR}/piece_points_builder)
