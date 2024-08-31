# Define the source and destination directories
set(DATA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Get a list of all files in the data directory
file(GLOB DATA_FILES "${DATA_SOURCE_DIR}/*")

# Add a custom command to copy the files if any of them change
add_custom_command(
    OUTPUT ${DATA_DESTINATION_DIR}/.copy_done
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${DATA_SOURCE_DIR} ${DATA_DESTINATION_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${DATA_DESTINATION_DIR}/.copy_done
    DEPENDS ${DATA_FILES}
    COMMENT "Copying data directory"
)

# Create a custom target to depend on the copy command
add_custom_target(copy_data ALL DEPENDS ${DATA_DESTINATION_DIR}/.copy_done)