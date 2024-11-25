add_library(move_evaluators_for_concepts move_evaluators_for_concepts.cpp)

target_link_libraries(
    move_evaluators_for_concepts PUBLIC
    nlohmann_json
    utility_functs)