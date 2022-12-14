#ifndef _PIECE_POINTS_BUILDER_
#define _PIECE_POINTS_BUILDER_

#include <unordered_map>
#include "board_components.hpp"
#include "piece_points_raw.hpp"

using namespace std;
using namespace board_components;
using namespace piece_points_raw;


class PiecePointsBuilder {
    public:
    PiecePositionPoints_t ComputePieceNetPositionPoints(
        int base_val,
        PiecePositionPoints_t position_offsets);
    
    TeamPositionPoints_t ComputeBlackPositionPoints(
        TeamBasePoints_t black_base_pts,
        TeamPositionPoints_t black_position_offsets);

    PiecePositionPoints_t FlipBoardDirection(
        PiecePositionPoints_t orig_piece_pts);
    
    TeamPositionPoints_t ComputeRedPositionPoints(
        TeamPositionPoints_t black_position_points);
    
    GamePositionPoints_t BuildGamePositionPoints(
        TeamPositionPoints_t black_points);
    
    GamePositionPoints_t BuildGamePositionPoints(
        TeamBasePoints_t black_base_points,
        TeamPositionPoints_t black_position_offsets);
    
};

const auto DEFAULT_GAME_POINTS = PiecePointsBuilder()
    .BuildGamePositionPoints(kBasePointsICGA2004, kAllOffsetsICGA2004);

#endif // _PIECE_POINTS_BUILDER_