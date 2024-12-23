add_executable(
    move_evaluator_random_for_concepts_test
    move_evaluator_random_for_concepts_test.cpp
    )

target_link_libraries(
    move_evaluator_random_for_concepts_test PRIVATE
    GTest::gtest_main
    nlohmann_json
    moveselection
    )