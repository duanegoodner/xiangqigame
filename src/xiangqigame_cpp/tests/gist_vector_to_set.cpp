#include <iostream>
#include <vector>
#include <set>

struct BoardSpace {
    int rank;
    int file;
};

struct Move {
    BoardSpace start;
    BoardSpace end;
};

bool moveComparator(const Move& lhs, const Move& rhs) {
    if (lhs.start.rank != rhs.start.rank)
        return lhs.start.rank < rhs.start.rank;
    if (lhs.start.file != rhs.start.file)
        return lhs.start.file < rhs.start.file;
    if (lhs.end.rank != rhs.end.rank)
        return lhs.end.rank < rhs.end.rank;
    return lhs.end.file < rhs.end.file;
}

std::set<Move, bool(*)(const Move&, const Move&)> convertToSet(const std::vector<Move>& moves) {
    std::set<Move, bool(*)(const Move&, const Move&)> moveSet(moveComparator);
    
    for (const auto& move : moves) {
        moveSet.insert(move);
    }
    
    return moveSet;
}

int main() {
    std::vector<Move> moves = {
        {{1, 2}, {3, 4}},
        {{1, 2}, {3, 4}},
        {{2, 3}, {4, 5}},
        {{1, 3}, {3, 4}},
    };
    
    auto uniqueMoves = convertToSet(moves);
    
    for (const auto& move : uniqueMoves) {
        std::cout << "Start: (" << move.start.rank << ", " << move.start.file << ") "
                  << "End: (" << move.end.rank << ", " << move.end.file << ")\n";
    }
    
    return 0;
}
