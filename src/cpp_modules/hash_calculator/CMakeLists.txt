
add_library(hash_calculator hash_calculator.cpp hash_calculator.hpp)
target_link_libraries(
        hash_calculator PRIVATE
        board_components
        board_utilities
        common
        utility_functs
        game_board
        piece_points)
target_include_directories(
        hash_calculator PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/nlohmann_json/
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_utilities/
        ${PROJECT_SOURCE_DIR}/board_components/
        ${PROJECT_SOURCE_DIR}/game_board/)


add_executable(
        create_zobrist_keys create_zobrist_keys.cpp hash_calculator.hpp)
target_link_libraries(
        create_zobrist_keys PRIVATE
        board_components
        board_utilities
        hash_calculator
        game_board
        )
target_include_directories(
        create_zobrist_keys PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/nlohmann_json/
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_components/
        ${PROJECT_SOURCE_DIR}/game_board/
)

add_executable(
        hash_calculator_test
        hash_calculator_test.cpp)
target_link_libraries(
        hash_calculator_test PRIVATE
        hash_calculator
        nlohmann_json
        board_components
        board_utilities
        utility_functs
        game_board
        GTest::GTest
        GTest::Main)
target_include_directories(
        hash_calculator_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_components/
        ${PROJECT_SOURCE_DIR}game_board/
        # ${PROJECT_SOURCE_DIR}/nlohmann_json/
        )


