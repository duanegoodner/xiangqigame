#include <algorithm>
#include <iostream>
#include "piece_points.hpp"

using namespace std;
using namespace board_components;
using namespace piece_points_raw;

TeamPositionPoints_t compute_net_pts(
    TeamBasePoints_t base_pts,
    TeamPositionPoints_t offsets)
{
    TeamPositionPoints_t all_net_pts;

    for (auto entry : base_pts) {
        PiecePositionPoints_t cur_net_pts;
        for (auto rank = 0; rank < kNumRanks; rank++) {
            for (auto file = 0; file < kNumFiles; file++) {
                cur_net_pts[rank][file] = entry.second + offsets.at(entry.first)[rank][file];
            }
        }
        all_net_pts[entry.first] = cur_net_pts;
    }
    return all_net_pts;

};

TeamPositionPoints_t flip_board_direction(TeamPositionPoints_t orig_pts_arrays) {
    TeamPositionPoints_t flipped_pts_arrays = orig_pts_arrays;

    for (auto array : flipped_pts_arrays) {
        reverse(array.second.begin(), array.second.end());
        flipped_pts_arrays[array.first] = array.second;
    }

    return flipped_pts_arrays;

}

PiecePoints::PiecePoints(
    TeamBasePoints_t black_base_points,
    TeamPositionPoints_t black_offsets)
    : black_piece_points_{compute_net_pts(
        black_base_points, black_offsets)}
    , red_piece_points_{flip_board_direction(compute_net_pts(
        black_base_points, black_offsets))} {};

PiecePoints::PiecePoints(
    TeamBasePoints_t black_base_pts,
    TeamPositionPoints_t black_offsets,
    TeamBasePoints_t red_base_pts,
    TeamPositionPoints_t red_offsets) 
    : black_piece_points_{compute_net_pts(
        black_base_pts, black_offsets)}
    , red_piece_points_{compute_net_pts(
        red_base_pts, red_offsets)} {};


// int main()
// {
//     auto my_piece_pts = PiecePoints(kBasePointsICGA2004, kAllOffsetsICGA2004);

//     cout << &(my_piece_pts.black_piece_points_[PieceType::kGen][0][0]) << endl;
//     cout << &(my_piece_pts.black_piece_points_[PieceType::kAdv][0][0]) << endl;
//     cout << &(my_piece_pts.black_piece_points_[PieceType::kEle][0][0]) << endl;
//     cout << &(my_piece_pts.black_piece_points_[PieceType::kCha][0][0]) << endl;
//     cout << &(my_piece_pts.black_piece_points_[PieceType::kHor][0][0]) << endl;
//     cout << &(my_piece_pts.black_piece_points_[PieceType::kCan][0][0]) << endl;
//     cout << &(my_piece_pts.black_piece_points_[PieceType::kSol][0][0]) << endl;
//     cout << "done with array start addresses" << endl;

//     return 0;
// };
