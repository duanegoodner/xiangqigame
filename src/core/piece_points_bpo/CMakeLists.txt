add_library(
    piece_points_bpo
    piece_points_bpo.cpp)

    target_link_libraries(
        piece_points_bpo PRIVATE
        json_utility_nlohmann
        utility_functs
    )
