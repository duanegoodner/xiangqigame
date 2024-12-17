add_executable(
    terminal_output_test
    terminal_output_test.cpp
    )

target_link_libraries(
    terminal_output_test PRIVATE
    GTest::gtest_main
    terminal_output
    )