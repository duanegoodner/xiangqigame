add_library(
    board_utilities INTERFACE board_utilities.hpp)

target_link_libraries(
    board_utilities INTERFACE
    board_components)

target_include_directories(
    board_utilities INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR})

