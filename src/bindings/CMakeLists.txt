# add_compile_options(-Wall -Wextra -O3)


pybind11_add_module(xiangqigame_core xiangqigame_core.cpp)
target_link_libraries(
    xiangqigame_core PRIVATE
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
    xiangqigame_core PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/game_board
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/core/json_internal
    ${PROJECT_SOURCE_DIR}/src/core/hash_calculator
    ${PROJECT_SOURCE_DIR}/src/core/move_calculator
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators
    ${PROJECT_SOURCE_DIR}/src/core/piece_points
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_spec
    ${CMAKE_CURRENT_SOURCE_DIR})

if(SKBUILD)
    install(TARGETS xiangqigame_core LIBRARY DESTINATION .)
endif()