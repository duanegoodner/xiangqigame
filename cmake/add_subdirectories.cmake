
add_subdirectory(${SOURCE_CODE_DIR}/game)
add_subdirectory(${SOURCE_CODE_DIR}/gameboard)
add_subdirectory(${SOURCE_CODE_DIR}/jsonio)
add_subdirectory(${SOURCE_CODE_DIR}/jsonvalidation)
add_subdirectory(${SOURCE_CODE_DIR}/moveselection)
add_subdirectory(${SOURCE_CODE_DIR}/movetranslation)
add_subdirectory(${SOURCE_CODE_DIR}/piecepoints)
add_subdirectory(${SOURCE_CODE_DIR}/terminalout)
add_subdirectory(${SOURCE_CODE_DIR}/utility_functs)
add_subdirectory(src/bindings)


if(BUILD_TESTS)
    add_subdirectory(tests/core)
endif()

if(BUILD_PROTOTYPES)
    add_subdirectory(prototypes)
endif()


if(NOT SKBUILD)
    add_subdirectory(src/data)
endif()