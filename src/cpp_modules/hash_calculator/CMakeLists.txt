add_executable(
        create_zobrist_keys create_zobrist_keys.cpp hash_calculator.hpp)

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

add_library(hash_calculator hash_calculator.cpp hash_calculator.hpp)
target_link_libraries(
        hash_calculator PUBLIC
        board_components
        utility_functs)
target_include_directories(
        hash_calculator PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/nlohmann_json/
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_components/)

add_executable(
        hash_calculator_test
        hash_calculator_test.cpp)
target_link_libraries(
        hash_calculator_test PUBLIC
        hash_calculator
        board_components
        utility_functs
        game_board
        GTest::GTest
        GTest::Main)
target_include_directories(
        hash_calculator_test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_components/
        ${PROJECT_SOURCE_DIR}game_board/
        ${PROJECT_SOURCE_DIR}/nlohmann_json/)


