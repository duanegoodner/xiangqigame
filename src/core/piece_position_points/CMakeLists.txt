add_library(
    piece_position_points
    piece_position_points.cpp)

target_link_libraries(
    piece_position_points PRIVATE
    utility_functs
    moveselection
    piece_points_bpo
)


