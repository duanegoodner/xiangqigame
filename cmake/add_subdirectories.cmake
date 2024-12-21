
add_subdirectory(${SOURCE_CODE_DIR}/game)
add_subdirectory(${SOURCE_CODE_DIR}/game_board)
add_subdirectory(${SOURCE_CODE_DIR}/game_board_for_concepts)

# add_subdirectory(${SOURCE_CODE_DIR}/game_factory)
add_subdirectory(${SOURCE_CODE_DIR}/json_utility_nlohmann)
add_subdirectory(${SOURCE_CODE_DIR}/json_validation)
add_subdirectory(${SOURCE_CODE_DIR}/move_evaluators)
add_subdirectory(${SOURCE_CODE_DIR}/move_evaluator_human_for_concepts)
add_subdirectory(${SOURCE_CODE_DIR}/move_evaluator_minimax_for_concepts)
add_subdirectory(${SOURCE_CODE_DIR}/move_evaluator_random_for_concepts)
add_subdirectory(${SOURCE_CODE_DIR}/move_calculator)
add_subdirectory(${SOURCE_CODE_DIR}/move_translator)
add_subdirectory(${SOURCE_CODE_DIR}/piece_moves)
add_subdirectory(${SOURCE_CODE_DIR}/piece_position_points)
add_subdirectory(${SOURCE_CODE_DIR}/piece_position_points_for_concepts)
add_subdirectory(${SOURCE_CODE_DIR}/piece_points_bpo)
add_subdirectory(${SOURCE_CODE_DIR}/terminal_output)
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