add_library(
    points_containers points_containers.cpp)
target_link_libraries(
    points_containers PRIVATE
    common
    utility_functs
)
target_include_directories(
    points_containers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/utility_functs
)

add_executable(points_containers_test points_containers_test.cpp)
target_link_libraries(
    points_containers_test PRIVATE
    GTest::gtest_main
    common
    json_interface
    json_io
    points_containers
    utility_functs
    )
target_include_directories(
    points_containers_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/json_interface
    ${PROJECT_SOURCE_DIR}/src/core/json_io
    ${PROJECT_SOURCE_DIR}/src/core/utility_functs
)