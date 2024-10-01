# add_library(
#         key_generator INTERFACE
#         key_generator.hpp)
# target_include_directories(key_generator INTERFACE
#         ${PROJECT_SOURCE_DIR}/src/core/common/)

add_library(
        hash_calculator INTERFACE
        # hash_calculator.hpp
        )
target_link_libraries(
        hash_calculator INTERFACE
        # key_generator
        utility_functs
        game_board
        piece_points
        nlohmann_json::nlohmann_json)
target_include_directories(
        hash_calculator INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR})

# **** No longer compiling this b/c json implementation does not suppport 128-bit ints **** 
# add_executable(
#         create_zobrist_keys create_zobrist_keys.cpp hash_calculator.hpp)
# target_link_libraries(
#         create_zobrist_keys PRIVATE
#         board_components
#         board_utilities
#         hash_calculator
#         game_board
#         nlohmann_json::nlohmann_json
# )
# target_include_directories(
#         create_zobrist_keys PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}
#         ${PROJECT_SOURCE_DIR}/src/core/utility_functs/
#         ${PROJECT_SOURCE_DIR}/src/core/board_components/
#         ${PROJECT_SOURCE_DIR}/src/core/game_board/
# )









