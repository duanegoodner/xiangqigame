
add_library(
    game_board game_board.cpp)

target_link_libraries(
    game_board PRIVATE
    board_components
    board_utilities
    common
    move_calculator
    nlohmann_json
    hash_calculator
    utility_functs)

# target_link_libraries(
#     game_board PUBLIC
#     board_components
#     board_utilities)

target_include_directories(
    game_board PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/hash_calculator)
