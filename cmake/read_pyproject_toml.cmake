execute_process(
    COMMAND ${Python_EXECUTABLE} ${PROJECT_SOURCE_DIR}/pyproject_toml_reader.py
    OUTPUT_VARIABLE PYPROJECT_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Parse the output from the script
string(REGEX MATCH "name=([^\n]+)" _match_name "${PYPROJECT_OUTPUT}")
set(PROJECT_NAME_FROM_TOML "${CMAKE_MATCH_1}")

string(REGEX MATCH "version=([^\n]+)" _match_version "${PYPROJECT_OUTPUT}")
set(PROJECT_VERSION_FROM_TOML "${CMAKE_MATCH_1}")

string(REGEX MATCH "description=([^\n]+)" _match_description "${PYPROJECT_OUTPUT}")
set(PROJECT_DESCRIPTION_FROM_TOML "${CMAKE_MATCH_1}")

# Use these variables as needed
message(STATUS "Project name from pyproject.toml: ${PROJECT_NAME_FROM_TOML}")
message(STATUS "Project version from pyproject.toml: ${PROJECT_VERSION_FROM_TOML}")
message(STATUS "Project description from pyproject.toml: ${PROJECT_DESCRIPTION_FROM_TOML}")