add_library(
    piece_points
    piece_points.cpp)

target_link_libraries(
    piece_points PRIVATE
    common
    json_internal
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    utility_functs
    minimax_evaluator
    piece_points_spec
)

target_include_directories(
    piece_points PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/board_components
    ${PROJECT_SOURCE_DIR}/src/core/utility_functs
    ${PROJECT_SOURCE_DIR}/src/core/json_internal
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/core/json_internal)

add_executable(
    bpo_json_to_raw_json bpo_json_to_raw_json.cpp
)
target_link_libraries(
    bpo_json_to_raw_json PRIVATE
    common
    piece_points
    piece_points_spec
    minimax_evaluator
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    utility_functs
)
target_include_directories(
    bpo_json_to_raw_json PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/board_components
    ${PROJECT_SOURCE_DIR}/src/core/json_internal
    ${PROJECT_SOURCE_DIR}/src/core/utility_functs
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_spec)

# add_executable(
# piece_points_raw_json_writer piece_points_raw_json_writer.cpp
# )
# target_link_libraries(
# piece_points_raw_json_writer PRIVATE
# common
# piece_points
# nlohmann_json
# utility_functs
# )
# target_include_directories(
# piece_points_raw_json_writer PRIVATE
# ${CMAKE_CURRENT_SOURCE_DIR}
# ${PROJECT_SOURCE_DIR}/board_components
# ${PROJECT_SOURCE_DIR}/utility_functs
# # ${PROJECT_SOURCE_DIR}/game_board
# )
