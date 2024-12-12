add_library(move_evaluator_minimax_for_concepts move_evaluator_minimax_for_concepts.cpp)

target_link_libraries(
    move_evaluator_minimax_for_concepts PUBLIC
    nlohmann_json
    utility_functs)