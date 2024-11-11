add_executable(json_schema_validate json_schema_validate.cpp)
target_link_libraries(
    json_schema_validate
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    utility_functs
    )

add_executable(my_schemas my_schemas.cpp)
target_link_libraries(
    my_schemas
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    json_utility_nlohmann
    utility_functs
    )

