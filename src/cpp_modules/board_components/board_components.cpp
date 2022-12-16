#include <vector>
#include "board_components.hpp"

using namespace board_components;
using namespace std;

MoveCollection::MoveCollection() : moves{} {};
MoveCollection::MoveCollection(vector<Move> my_moves) : moves{my_moves} {};
MoveCollection::MoveCollection(size_t reserve_size) : moves{} {
        moves.reserve(reserve_size);
}
bool MoveCollection::Contains(Move move) {
        for (auto entry : moves) {
            if ((move.start == entry.start) && (move.end == entry.end)) {
                return true;
            }
        }
        return false;
    }
void MoveCollection::Append(Move move) {
        moves.emplace_back(move);
    }
void MoveCollection::Concat(vector<Move> other_moves) {
        moves.insert(moves.end(), other_moves.begin(), other_moves.end());
    }
void MoveCollection::Concat(MoveCollection other) {
        moves.insert(moves.end(), other.moves.begin(), other.moves.end());
    }

