add_library(
    points_containers points_containers.cpp)
target_link_libraries(
    points_containers PRIVATE
    # common
    utility_functs
)
target_include_directories(
    points_containers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/utility_functs
)

