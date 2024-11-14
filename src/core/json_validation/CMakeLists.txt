add_library(json_validation json_validation.cpp)
target_link_libraries(
    json_validation PUBLIC
    nlohmann_json
    nlohmann_json_schema_validator)

add_executable(json_schema_validate json_schema_validate.cpp)
target_link_libraries(
    json_schema_validate
    nlohmann_json
    nlohmann_json_schema_validator
    utility_functs
    )

add_executable(my_schemas my_schemas.cpp)
target_link_libraries(
    my_schemas
    json_validation
    utility_functs
    )

