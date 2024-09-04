add_library(gist_key_generator_lib INTERFACE gist_key_generator.hpp)

target_include_directories(
    gist_key_generator_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(gist_key_generator gist_key_generator.cpp gist_key_generator.hpp)
target_link_libraries(gist_key_generator PRIVATE gist_key_generator_lib)

