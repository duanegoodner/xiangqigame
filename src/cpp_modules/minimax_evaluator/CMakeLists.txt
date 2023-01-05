# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

add_library(
    minimax_evaluator
    minimax_evaluator.cpp)

target_link_libraries(
    minimax_evaluator PRIVATE
    board_components
    board_utilities
    common
    game_board
    piece_points
    move_calculator
    nlohmann_json
    utility_functs
    )

target_include_directories(
    minimax_evaluator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/piece_points
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/move_selector)


    