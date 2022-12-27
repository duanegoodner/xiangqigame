add_executable(
        create_zobrist_keys create_zobrist_keys.cpp zobrist_hash.hpp)

target_link_libraries(
        create_zobrist_keys PUBLIC
        board_components
        board_utilities
        game_board
        )

target_include_directories(
        create_zobrist_keys PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/nlohmann_json/
        ${PROJECT_SOURCE_DIR}/utility_functs/
        ${PROJECT_SOURCE_DIR}/board_components/
        ${PROJECT_SOURCE_DIR}/board_utilities/
        ${PROJECT_SOURCE_DIR}/game_board/
)