add_executable(move_translator_test move_translator_test.cpp)
target_link_libraries(move_translator_test PRIVATE GTest::gtest_main movetranslation)

# Define the source and destination paths
set(ALGEBRAIC_MOVES_FILE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/algebraic_moves.txt)
set(ALGEBRAIC_MOVES_FILE_DEST ${TEST_DATA_DESTINATION_DIR}/algebraic_moves.txt)

# Add a custom command to ensure the directory exists and copy the file
add_custom_command(
    OUTPUT ${ALGEBRAIC_MOVES_FILE_DEST}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_DATA_DESTINATION_DIR} &&
            ${CMAKE_COMMAND} -E copy ${ALGEBRAIC_MOVES_FILE_SOURCE} ${ALGEBRAIC_MOVES_FILE_DEST}
    DEPENDS ${ALGEBRAIC_MOVES_FILE_SOURCE}
    COMMENT "Preparing test data: ensuring ${TEST_DATA_DESTINATION_DIR} exists and copying algebraic_moves.txt"
)

# Add a single custom target for both steps
add_custom_target(prepare_test_data ALL DEPENDS ${ALGEBRAIC_MOVES_FILE_DEST})
