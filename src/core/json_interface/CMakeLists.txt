add_library(
    json_interface json_interface.cpp)
# target_link_libraries(
#     json_interface PRIVATE
#     common)
target_include_directories(
    json_interface PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})
