add_library(
    minimax_evaluator
    minimax_evaluator.cpp)

target_link_libraries(
    minimax_evaluator PRIVATE
    board_components
    common
    utility_functs
    nlohmann_json::nlohmann_json
    )

target_include_directories(
    minimax_evaluator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/utility_functs
    )


    