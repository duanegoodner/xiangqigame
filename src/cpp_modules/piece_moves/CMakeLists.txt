add_library(
    piece_moves piece_moves.cpp
)

target_link_libraries(
    piece_moves PRIVATE
    board_components
    board_utilities
    common
    move_calculator
)

target_include_directories(
    piece_moves PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/move_calculator
)