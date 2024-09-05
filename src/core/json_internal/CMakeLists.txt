add_library(json_internal json_internal.cpp)
target_link_libraries(
    json_internal PUBLIC
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator)
target_include_directories(
    json_internal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

