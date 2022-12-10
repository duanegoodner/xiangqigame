#include <array>
#include <iostream>

#include "shared_components.hpp"
#include "piece_points_raw.hpp"

using namespace std;
using namespace board_components;
using namespace piece_points_raw;

typedef array<PiecePositionPoints_t, 8> TeamPtsArray_t;


int main() {
  
    TeamPtsArray_t black_pts_;

    cout << black_pts_.begin() << ", " << black_pts_.end() << endl;
    cout << endl;

    for (auto piece_idx = 0; piece_idx < 8; piece_idx++) {
        cout << (black_pts_[piece_idx].begin()) << ", "
            << (black_pts_[piece_idx].end())
            << endl;
    }

    auto black_pts_start = &(black_pts_[PieceType::kNnn][0][0]);
    auto black_pts_end = &(black_pts_[PieceType::kSol][10][9]);

    cout << "done" << endl;

    

    return 0;
}