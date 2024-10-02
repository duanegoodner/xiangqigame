add_library(
    utility_functs utility_functs.cpp)

target_link_libraries(
    utility_functs PUBLIC
    nlohmann_json::nlohmann_json
     
)
