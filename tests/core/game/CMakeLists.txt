# add_executable(player_spec_builder_test player_spec_builder_test.cpp)
# target_link_libraries(
#     player_spec_builder_test PRIVATE
#     GTest::gtest_main
#     jsonio
#     utilities
# )

add_executable(game_test game_test.cpp)
target_link_libraries(
    game_test PRIVATE
    GTest::gtest_main
    game
    nlohmann_json
    utilities
    gameboard
    moveselection
    terminalout
    )

add_executable(game_runner_test game_runner_test.cpp)
target_link_libraries(
    game_runner_test PRIVATE
    GTest::gtest_main
    game
    jsonio
    terminalout
    utilities
    )


### Copy files for scripted game to build directory

# Build list of files to be copied
set(SCRIPTED_MOVE_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/scripted_game_black_moves.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/scripted_game_red_moves.txt
    )

set(SCRIPTED_MOVE_FILES_DESTINATION_DIR ${TEST_DATA_DESTINATION_DIR})

# Add a custom command to ensure the destination directory exists
add_custom_command(
    OUTPUT ${SCRIPTED_MOVE_FILES_DESTINATION_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SCRIPTED_MOVE_FILES_DESTINATION_DIR}
    COMMENT "Ensuring the test_data directory exists"
)

# Generate commands to copy each file
foreach(SRC_FILE IN LISTS SCRIPTED_MOVE_SOURCE_FILES)
    get_filename_component(FILENAME ${SRC_FILE} NAME)
    set(DEST_FILE ${SCRIPTED_MOVE_FILES_DESTINATION_DIR}/${FILENAME})
    
    add_custom_command(
        OUTPUT ${DEST_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy ${SRC_FILE} ${DEST_FILE}
        DEPENDS ${SRC_FILE} ${DESTINATION_DIR}
        COMMENT "Copying ${FILENAME} to the test_data directory"
    )

    # Collect destination files for the custom target
    list(APPEND DEST_FILES ${DEST_FILE})
endforeach()

# Create a custom target to handle all copying
add_custom_target(copy_scripted_game_moves ALL DEPENDS ${DEST_FILES})


