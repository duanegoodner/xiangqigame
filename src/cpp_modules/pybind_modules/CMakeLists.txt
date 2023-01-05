# add_compile_options(-Wall -Wextra -O3)


pybind11_add_module(GameBoardPy game_board_py.cpp)
target_link_libraries(
    GameBoardPy PRIVATE
    board_components
    board_utilities
    common
    game_board
    nlohmann_json
    utility_functs
    )

target_include_directories(
    GameBoardPy PRIVATE
    ${PROJECT_SOURCE_DIR}/game_board
    # ${PROJECT_SOURCE_DIR}/board_components
    # ${PROJECT_SOURCE_DIR}/board_utilities
    # ${PROJECT_SOURCE_DIR}/minimax_evaluator
    ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS GameBoardPy DESTINATION .)

pybind11_add_module(MoveSelectorPy move_selector_py.cpp)
target_link_libraries(
    MoveSelectorPy PRIVATE
    board_components
    board_utilities
    common
    move_selector
    move_calculator
    nlohmann_json
    piece_points
    game_board
    minimax_evaluator
    piece_points
    utility_functs
    )
    
target_include_directories(
    MoveSelectorPy PUBLIC
    ${PROJECT_SOURCE_DIR}/move_selector
    # ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/minimax_evaluator
    # ${PROJECT_SOURCE_DIR}/move_calculator
    ${PROJECT_SOURCE_DIR}/piece_points
    # ${PROJECT_SOURCE_DIR}/utility_functs
    ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS MoveSelectorPy DESTINATION .)
