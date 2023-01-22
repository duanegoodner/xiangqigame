add_library(
    move_selector
    move_selector.cpp)

target_link_libraries(
    move_selector PRIVATE
    board_components
    # piece_points
    utility_functs
    nlohmann_json::nlohmann_json
)

target_include_directories(
    move_selector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/utility_functs
)
