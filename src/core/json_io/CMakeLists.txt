
add_library(json_io json_io.cpp)
target_link_libraries(
    json_io PUBLIC
    json_interface
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator)
target_link_libraries(
    json_io PRIVATE
    common
)
target_include_directories(
    json_io PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/json_interface)

add_executable(json_io_test json_io_test.cpp)
target_link_libraries(
    json_io_test PRIVATE
    GTest::gtest_main
    common
    json_interface
    json_io
    utility_functs)
target_include_directories(
    json_io_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/json_interface
    ${PROJECT_SOURCE_DIR}/src/core/utility_functs
    )
