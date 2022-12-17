#ifndef _GAME_PIECE_
#define _GAME_PIECE_

namespace game_piece
{

    enum PieceType : int
    {
        kNnn = 0,
        kGen = 1,
        kAdv = 2,
        kEle = 3,
        kHor = 4,
        kCha = 5,
        kCan = 6,
        kSol = 7
    };

    enum PieceColor : int
    {
        kRed = -1,
        kNul = 0,
        kBlk = 1
    };

    const int kNumPieceTypeVals = 8;

}  // namespace game_piece

#endif