#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_board_for_concepts.hpp>
#include <memory>
#include <gameboard/piece_moves.hpp>
#include <stdexcept>
#include <utilities/utility_functs.hpp>

namespace gameboard {

// //! Starting board represented as 2-D array of integers.
// //! Can be converted to array of GamePiece objects by
// //! board_utilities::int_board_to_game_pieces.
const BoardMapInt_t kStandardInitialBoard = {{
    {5, 4, 3, 2, 1, 2, 3, 4, 5},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 6, 0, 0, 0, 0, 0, 6, 0},
    {7, 0, 7, 0, 7, 0, 7, 0, 7},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-7, 0, -7, 0, -7, 0, -7, 0, -7},
    {0, -6, 0, 0, 0, 0, 0, -6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-5, -4, -3, -2, -1, -2, -3, -4, -5},
}};

// //! Max allowed repetitions of prohibited move sequence lengths.
const int kRepeatPeriodsToCheck[3] = {2, 3, 4};

// //! Repeated move sequence lengths forbidden under move repetition rules.
// //! If kRepeatPeriodsToCheck = {2, 3, 4} and kRepeatPeriodsMaxAllowed = 2, then the
// //! following sequences are probibited:
// //! ABABAB, ABCABCABC, ABCDABCDABCD, ABCDEABCDEABCDE, ABCDEFABCDEFABCDEF
const int kRepeatPeriodsMaxAllowed = 2;

const int kMaxMovesWithoutCapture = 120;

} // namespace gameboard
