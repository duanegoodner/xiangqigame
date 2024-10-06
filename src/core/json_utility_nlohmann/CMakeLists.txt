add_library(
    json_utility_nlohmann json_utility_nlohmann.cpp
)

target_link_libraries(
    json_utility_nlohmann PUBLIC
    nlohmann_json
    # nlohmann_json::nlohmann_json
    )
