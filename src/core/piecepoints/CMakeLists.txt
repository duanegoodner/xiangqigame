add_library(
    piecepoints
    piece_points_bpo.cpp
    piece_position_points.cpp
    piece_position_points_for_concepts.cpp
    )

target_link_libraries(piecepoints PUBLIC jsonio)