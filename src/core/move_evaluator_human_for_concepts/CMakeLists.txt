add_library(
    move_evaluator_human_for_concepts
    move_evaluator_human_for_concepts.cpp
    )

target_link_libraries(
    move_evaluator_human_for_concepts
    PUBLIC
    game_board_for_concepts move_translator
    )

