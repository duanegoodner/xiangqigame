add_library(
    minimax_evaluator
    minimax_evaluator.cpp)

target_link_libraries(
    minimax_evaluator PRIVATE
    board_components
    common
    move_selector
    piece_points
    utility_functs
    )

target_include_directories(
    minimax_evaluator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/piece_points
    ${PROJECT_SOURCE_DIR}/move_selector
    )


    