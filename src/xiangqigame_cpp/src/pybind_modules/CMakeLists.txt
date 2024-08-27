# add_compile_options(-Wall -Wextra -O3)


pybind11_add_module(XiangqigamePy game_board_py.cpp)
target_link_libraries(
    XiangqigamePy PRIVATE
    board_components
    board_utilities
    common
    hash_calculator
    game_board
    json_internal
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    piece_points
    piece_points_spec
    utility_functs
    )

target_include_directories(
    XiangqigamePy PRIVATE
    ${PROJECT_SOURCE_DIR}/src/game_board
    ${PROJECT_SOURCE_DIR}/src/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/json_internal
    ${PROJECT_SOURCE_DIR}/src/hash_calculator
    ${PROJECT_SOURCE_DIR}/src/move_calculator
    ${PROJECT_SOURCE_DIR}/src/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/src/piece_points
    ${PROJECT_SOURCE_DIR}/src/piece_points_spec
    ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS XiangqigamePy DESTINATION .)
