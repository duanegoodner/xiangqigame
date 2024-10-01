add_library(
    piece_points_spec
    piece_points_spec.cpp)

    target_link_libraries(
        piece_points_spec PRIVATE
        # common
        json_internal
        nlohmann_json
        utility_functs
        nlohmann_json::nlohmann_json
    )

target_include_directories(
    piece_points_spec PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/utility_functs
    ${PROJECT_SOURCE_DIR}/src/core/json_internal
)

# add_executable(
#     points_containers_test
#     points_containers_test.cpp
# )
# target_link_libraries(
#     points_containers_test PRIVATE
#     common
#     nlohmann_json
#     utility_functs
#     nlohmann_json::nlohmann_json
#     json_internal
#     piece_points_spec
#     GTest::gtest_main
# )
# target_include_directories(
#     points_containers_test PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${PROJECT_SOURCE_DIR}/src/utility_functs
#     ${PROJECT_SOURCE_DIR}/src/json_internal)