add_library(
    utility_functs INTERFACE utility_functs.hpp)

target_include_directories(
    utility_functs INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR})