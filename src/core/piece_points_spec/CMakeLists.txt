add_library(
    piece_points_spec
    piece_points_spec.cpp)

    target_link_libraries(
        piece_points_spec PRIVATE
        nlohmann_json
        utility_functs
        nlohmann_json::nlohmann_json
    )
