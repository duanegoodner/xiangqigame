#ifndef _MOVE_
#define _MOVE_

#include "board_components.hpp"

using namespace board_components;

namespace move
{

    struct Move
    {
        BoardSpace start;
        BoardSpace end;

        bool operator==(const Move other)
        {
            return (start == other.start) && (end == other.end);
        }
    };

    struct MoveCollection {
    vector<Move> moves;
    MoveCollection(): moves{} {};
    MoveCollection(vector<Move> my_moves) : moves{my_moves} {};
    MoveCollection(size_t reserve_size) : moves{} {
        moves.reserve(reserve_size);
    }
    bool Contains(Move move) {
        for (auto entry : moves) {
            if ((move.start == entry.start) && (move.end == entry.end)) {
                return true;
            }
        }
        return false;
    }
    void Append(Move move) {
        moves.emplace_back(move);
    }
    void Concat(vector<Move> other_moves) {
        moves.insert(moves.end(), other_moves.begin(), other_moves.end());
    }
    void Concat(MoveCollection other) {
        moves.insert(moves.end(), other.moves.begin(), other.moves.end());
    }
};

struct ExecutedMove {
    Move spaces;
    Piece_t moving_piece;
    Piece_t destination_piece;
};

}

#endif