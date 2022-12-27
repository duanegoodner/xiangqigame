add_library(
    move_calculator
    move_calculator.cpp)

target_link_libraries(
    move_calculator PUBLIC
    board_utilities
    board_components
    )

target_include_directories(
    move_calculator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(
move_calculator PUBLIC
${PROJECT_SOURCE_DIR}/board_components
${PROJECT_SOURCE_DIR}/board_utilities
)
