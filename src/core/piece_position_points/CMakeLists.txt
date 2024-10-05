add_library(
    piece_position_points
    piece_position_points.cpp)

target_link_libraries(
    piece_position_points PRIVATE
    utility_functs
    minimax_evaluator
    piece_points_spec
)

target_include_directories(
    piece_position_points PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators)

add_executable(
    bpo_json_to_raw_json bpo_json_to_raw_json.cpp
)
target_link_libraries(
    bpo_json_to_raw_json PRIVATE
    piece_position_points
    piece_points_spec
    minimax_evaluator
    utility_functs
)
target_include_directories(
    bpo_json_to_raw_json PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators)

