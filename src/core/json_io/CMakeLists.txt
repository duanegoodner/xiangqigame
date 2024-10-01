
add_library(json_io json_io.cpp)
target_link_libraries(
    json_io PUBLIC
    json_interface
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator)
