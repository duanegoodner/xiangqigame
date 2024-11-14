add_library(minimax_evaluator move_evaluators.cpp)

target_link_libraries(
    minimax_evaluator PUBLIC
    nlohmann_json
    utility_functs)



    