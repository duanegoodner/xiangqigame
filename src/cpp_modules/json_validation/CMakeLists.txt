add_executable(json_schema_validate json_schema_validate.cpp)
target_link_libraries(
    json_schema_validate
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    utility_functs
    )

target_include_directories(
    json_schema_validate PRIVATE
    ${PROJECT_SOURCE_DIR}/utility_functs/
)

add_executable(my_schemas my_schemas.cpp)
target_link_libraries(
    my_schemas
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    utility_functs
    )

target_include_directories(
    my_schemas PRIVATE
    ${PROJECT_SOURCE_DIR}/utility_functs/
)
