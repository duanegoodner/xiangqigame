add_library(
    piece_moves
    piece_moves.cpp piece_moves.hpp
)

target_link_libraries(
    piece_moves PRIVATE
    move_calculator
    board_utilities
    board_components
    common)

target_include_directories(
    piece_moves PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/board_components
    ${PROJECT_SOURCE_DIR}/src/board_utilities)