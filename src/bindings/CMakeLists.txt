# add_compile_options(-Wall -Wextra -O3)


pybind11_add_module(xiangqi_bindings bindings.cpp)
target_link_libraries(
    xiangqi_bindings PRIVATE
    game_board
    game_board_for_concepts
    move_evaluator_minimax_for_concepts
    piece_position_points
    piece_position_points_for_concepts
    piece_points_bpo
    utility_functs
    )

target_include_directories(
    xiangqi_bindings PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_bpo
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_bpo
    ${CMAKE_CURRENT_SOURCE_DIR})

if(SKBUILD)
    install(TARGETS xiangqi_bindings LIBRARY DESTINATION .)
endif()