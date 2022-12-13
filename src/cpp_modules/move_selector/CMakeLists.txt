# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

add_library(
    move_selector
    move_selector.cpp)

target_include_directories(
    move_selector PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/piece_points_builder
)

# add_executable(try_move_selector move_selector.cpp)
# target_link_libraries(
#     try_move_selector LINK_PUBLIC
#     game_board
#     minimax_evaluator
#     move_calculator
#     piece_points_builder
# )