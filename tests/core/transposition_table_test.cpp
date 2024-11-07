#include <game_board.hpp>
#include <gtest/gtest.h>
#include <zobrist.hpp>

class TranspositionTableEntryTest : public ::testing::Test {
protected:
};

TEST_F(TranspositionTableEntryTest, InitFromMinimaxCalcResultAndConfirmationKeys) {
  moveselection::MinimaxCalcResult empty_result{};
  std::array<uint64_t, 2> confirmation_keys{1234, 5678};
  boardstate::TranspositionTableEntryNew<uint64_t, 2>{empty_result, confirmation_keys};
}

TEST_F(TranspositionTableEntryTest, RecognizeConfirmationKeysMatch) {
  moveselection::MinimaxCalcResult empty_result{};
  std::array<uint64_t, 2> confirmation_keys{1234, 5678};
  boardstate::TranspositionTableEntryNew<uint64_t, 2> tr_table_entry{
      empty_result,
      confirmation_keys
  };
  EXPECT_TRUE(tr_table_entry.ConfirmationKeysMatchExpected(confirmation_keys));
}
