add_library(
    utility_functs utility_functs.cpp)

target_link_libraries(
    utility_functs PRIVATE
    nlohmann_json
     
)

target_include_directories(
    utility_functs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/nlohmann_json)