add_library(
    piece_points_spec
    piece_points_spec.cpp)

    target_link_libraries(
        piece_points_spec PRIVATE
        nlohmann_json
        utility_functs
        nlohmann_json::nlohmann_json
    )

target_include_directories(
    piece_points_spec PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
