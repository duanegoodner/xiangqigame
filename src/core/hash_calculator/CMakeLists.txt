
add_library(hash_calculator INTERFACE)
target_link_libraries(
        hash_calculator INTERFACE
        utility_functs
        game_board
        piece_position_points
        )

