add_library(
    piece_position_points
    piece_position_points.cpp)

target_link_libraries(
    piece_position_points PRIVATE
    utility_functs
    minimax_evaluator
    piece_points_bpo
)


