add_library(
    minimax_evaluator
    move_evaluators.cpp)

target_link_libraries(
    minimax_evaluator PRIVATE
    utility_functs
    nlohmann_json::nlohmann_json
    )

# .hpp file needs to access .tpp file CMAKE_CURRENT_SOURCE_DIR
target_include_directories(
    minimax_evaluator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )


    