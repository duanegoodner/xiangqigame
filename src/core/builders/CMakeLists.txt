add_library(builders builders.cpp)

target_link_libraries(
    builders PUBLIC
    game_board_for_concepts
    piece_position_points_for_concepts)