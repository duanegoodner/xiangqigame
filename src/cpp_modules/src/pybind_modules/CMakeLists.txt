# add_compile_options(-Wall -Wextra -O3)


pybind11_add_module(GameBoardPy game_board_py.cpp)
target_link_libraries(
    GameBoardPy PRIVATE
    board_components
    board_utilities
    common
    game_board
    json_internal
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    piece_points
    piece_points_spec
    utility_functs
    )

target_include_directories(
    GameBoardPy PRIVATE
    ${PROJECT_SOURCE_DIR}/src/game_board
    ${PROJECT_SOURCE_DIR}/src/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/json_internal
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${rapidjson_SOURCE_DIR}/include
    )

install(TARGETS GameBoardPy DESTINATION .)

pybind11_add_module(MoveSelectorPy move_selector_py.cpp)
target_link_libraries(
    MoveSelectorPy PRIVATE
    board_components
    board_utilities
    common
    hash_calculator
    game_board
    move_selector
    piece_points
    piece_points_spec
    utility_functs
    json_internal
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    )
    
target_include_directories(
    MoveSelectorPy PUBLIC
    ${PROJECT_SOURCE_DIR}/src/move_selector
    ${PROJECT_SOURCE_DIR}/src/minimax_evaluator
    ${PROJECT_SOURCE_DIR}/src/piece_points
    ${PROJECT_SOURCE_DIR}/src/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/game_board
    ${PROJECT_SOURCE_DIR}/src/hash_calculator
    ${PROJECT_SOURCE_DIR}/src/move_calculator
    ${PROJECT_SOURCE_DIR}/src/json_internal
    ${rapidjson_SOURCE_DIR}/include
    )

install(TARGETS MoveSelectorPy DESTINATION .)
