#include <algorithm>
#include <iostream>
#include "piece_points.hpp"

using namespace std;
using namespace board_components;
using namespace piece_points;


PiecePositionPoints_t PiecePointsBuilder::ComputePieceNetPositionPoints(
    int base_val, PiecePositionPoints_t position_offsets) {
        PiecePositionPoints_t net_points;
        for (auto rank = 0; rank < kNumRanks; rank++) {
            for (auto file = 0; file < kNumFiles; file++) {
                net_points[rank][file] = base_val + position_offsets[rank][file];
            }
        }
        return net_points;
    }

TeamPositionPoints_t PiecePointsBuilder::ComputeBlackPositionPoints(
    TeamBasePoints_t black_base_points,
    TeamPositionPoints_t black_position_offsets) {
        TeamPositionPoints_t black_team_points;
        
        for (auto piece : black_base_points) {
            black_team_points[piece.first] = ComputePieceNetPositionPoints(
                black_base_points[piece.first], black_position_offsets[piece.first]
            );
        }
        return black_team_points;
    }

PiecePositionPoints_t PiecePointsBuilder::FlipBoardDirection(
    PiecePositionPoints_t orig_piece_pts) {
        auto flipped_pts_array = orig_piece_pts;
        reverse(flipped_pts_array.begin(), flipped_pts_array.end());
        return flipped_pts_array;

    }

TeamPositionPoints_t PiecePointsBuilder::ComputeRedPositionPoints(
    TeamPositionPoints_t black_team_points) {
        TeamPositionPoints_t red_team_points = black_team_points;
        for (auto array : red_team_points) {
            red_team_points[array.first] = FlipBoardDirection(red_team_points[array.first]);
        }
        return red_team_points;
    }

GamePositionPoints_t PiecePointsBuilder::BuildGamePositionPoints(
    TeamPositionPoints_t black_points) {
        GamePositionPoints_t game_position_points;
        game_position_points[PieceColor::kBlk] = black_points;
        game_position_points[PieceColor::kRed] = ComputeRedPositionPoints(black_points);
        return game_position_points;
    }

GamePositionPoints_t PiecePointsBuilder::BuildGamePositionPoints(
        TeamBasePoints_t black_base_points,
        TeamPositionPoints_t black_position_offsets) {
            auto black_position_points = ComputeBlackPositionPoints(
                black_base_points, black_position_offsets);
            return BuildGamePositionPoints(black_position_points);

        }
        