add_library(
    jsonio json_utility_nlohmann.cpp
)

target_link_libraries(
    jsonio PUBLIC
    nlohmann_json
    )

