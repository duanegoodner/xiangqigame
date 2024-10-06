add_library(
    piece_points_spec
    piece_points_spec.cpp)

    target_link_libraries(
        piece_points_spec PRIVATE
        json_utility_nlohmann
        # nlohmann_json
        utility_functs
        # nlohmann_json::nlohmann_json
    )
