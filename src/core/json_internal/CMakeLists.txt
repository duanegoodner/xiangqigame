add_library(json_internal json_internal.cpp)
target_link_libraries(
    json_internal PUBLIC
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator)
target_include_directories(
    json_internal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
    json_internal_test
    json_internal_test.cpp)
target_link_libraries(
    json_internal_test PRIVATE
    json_internal
    utility_functs
    GTest::gtest_main)
target_include_directories(
    json_internal_test PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/utility_functs
)
