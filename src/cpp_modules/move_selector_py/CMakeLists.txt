add_compile_options(-Wall -Wextra -O3)

pybind11_add_module(MoveSelectorPy move_selector_py.cpp)
target_link_libraries(
    MoveSelectorPy PUBLIC
    move_selector
    game_board
    minimax_evaluator
    piece_points_builder)
target_include_directories(
    MoveSelectorPy PUBLIC
    ${PROJECT_SOURCE_DIR}/move_selector
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    # ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/piece_points_builder
    ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS MoveSelectorPy DESTINATION .)
