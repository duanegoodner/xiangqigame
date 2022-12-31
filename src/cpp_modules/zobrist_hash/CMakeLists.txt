add_executable(
        create_zobrist_keys create_zobrist_keys.cpp zobrist_hash.hpp)

target_link_libraries(
        create_zobrist_keys PUBLIC
        board_components
        board_utilities
        )

target_include_directories(
        create_zobrist_keys PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/nlohmann_json/
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_components/
)

add_library(zobrist_hash zobrist_hash.cpp)
target_link_libraries(
        zobrist_hash PUBLIC
        board_components
        utility_functs)
target_include_directories(
        zobrist_hash PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/nlohmann_json/
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_components/)

add_executable(
        zobrist_hash_test
        zobrist_hash_test.cpp)
target_link_libraries(
        zobrist_hash_test PUBLIC
        zobrist_hash
        board_components
        utility_functs
        game_board
        GTest::GTest
        GTest::Main)
target_include_directories(
        zobrist_hash_test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_components/
        ${PROJECT_SOURCE_DIR}game_board/
        ${PROJECT_SOURCE_DIR}/nlohmann_json/)


