# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-Wall -Wextra -O3)

pybind11_add_module(GameBoardPy game_board_py.cpp)
target_link_libraries(
    GameBoardPy PUBLIC
    game_board
    move_calculator)
target_include_directories(
    GameBoardPy PUBLIC
    ${PROJECT_SOURCE_DIR}/board_components
    ${PROJECT_SOURCE_DIR}/game_board
    ${PROJECT_SOURCE_DIR}/move_calculator
    ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS GameBoardPy DESTINATION .)