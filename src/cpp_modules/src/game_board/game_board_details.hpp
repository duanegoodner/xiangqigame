#ifndef C34222C4_BE2D_44FE_AFD0_CDFAFD25E156
#define C34222C4_BE2D_44FE_AFD0_CDFAFD25E156

#include <board_components.hpp>

using namespace board_components;

const BoardMapInt_t kStartingBoard{{
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

inline BoardMap_t int_board_to_game_pieces(const BoardMapInt_t int_board);

#endif /* C34222C4_BE2D_44FE_AFD0_CDFAFD25E156 */
