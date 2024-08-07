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
    move_selector
    piece_points_spec
)

target_include_directories(
    piece_points PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/board_components
    ${PROJECT_SOURCE_DIR}/src/utility_functs
    ${PROJECT_SOURCE_DIR}/src/json_internal
    ${PROJECT_SOURCE_DIR}/src/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/src/move_selector
    ${PROJECT_SOURCE_DIR}/src/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/json_internal)

add_executable(
    bpo_json_to_raw_json bpo_json_to_raw_json.cpp
)
target_link_libraries(
    bpo_json_to_raw_json PRIVATE
    common
    piece_points
    piece_points_spec
    minimax_evaluator
    move_selector
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    utility_functs
)
target_include_directories(
    bpo_json_to_raw_json PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/board_components
    ${PROJECT_SOURCE_DIR}/src/json_internal
    ${PROJECT_SOURCE_DIR}/src/utility_functs
    ${PROJECT_SOURCE_DIR}/src/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/src/move_selector
    ${PROJECT_SOURCE_DIR}/src/piece_points_spec)

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
