add_library(
    move_calculator
    move_calculator.cpp)

target_link_libraries(
    move_calculator PRIVATE
    board_utilities
    board_components
    common)

target_include_directories(
    move_calculator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/board_components
    ${PROJECT_SOURCE_DIR}/src/board_utilities)

