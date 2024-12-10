add_library(
    move_evaluator_random_for_concepts
    move_evaluator_random_for_concepts.cpp
    )

target_link_libraries(
    move_evaluator_random_for_concepts PUBLIC
    utility_functs game_board_for_concepts
    )