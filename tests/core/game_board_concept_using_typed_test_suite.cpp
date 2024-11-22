// game_board_builder_tests.cpp
#include <game_board_for_concepts.hpp> // Include your specific builder class
#include <shared_ptr_builder_tests.hpp>

// Define the type pairing for the builder and the objects it builds
using GameBoardBuilderTestTypes = ::testing::Types<
    BuilderObjectPair<gameboard::GameBoardBuilder, gameboard::GameBoardForConcepts>>;

INSTANTIATE_TYPED_TEST_SUITE_P(
    GameBoardBuilderTestInstance,
    SharedPtrBuilderTest,
    GameBoardBuilderTestTypes
);
