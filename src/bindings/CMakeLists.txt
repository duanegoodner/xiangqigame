# add_compile_options(-Wall -Wextra -O3)


pybind11_add_module(xiangqigame_core xiangqigame_core.cpp)
target_link_libraries(
    xiangqigame_core PRIVATE
    hash_calculator
    game_board
    piece_position_points
    piece_points_spec
    utility_functs
    )

target_include_directories(
    xiangqigame_core PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_spec
    ${PROJECT_SOURCE_DIR}/src/core/move_evaluators
    ${PROJECT_SOURCE_DIR}/src/core/piece_points_spec
    ${CMAKE_CURRENT_SOURCE_DIR})

if(SKBUILD)
    install(TARGETS xiangqigame_core LIBRARY DESTINATION .)
endif()