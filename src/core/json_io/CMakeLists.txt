
add_library(json_io json_io.cpp)
target_link_libraries(
    json_io PUBLIC
    json_interface
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator)
# target_link_libraries(
#     json_io PRIVATE
#     common
# )
target_include_directories(
    json_io PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/json_interface)

