add_library(
    moveselection
    move_evaluator_human_for_concepts.cpp
    move_evaluator_minimax_for_concepts.cpp
    move_evaluator_random_for_concepts.cpp
    move_evaluators.cpp
    )

target_link_libraries(
    moveselection PUBLIC
    gameboard
    movetranslation
    nlohmann_json
    utility_functs
    )