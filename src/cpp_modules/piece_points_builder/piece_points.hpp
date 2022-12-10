#include <unordered_map>
#include "shared_components.hpp"
#include "piece_points_raw.hpp"

using namespace std;
using namespace board_components;
using namespace piece_points_raw;




struct PiecePoints
{
    PiecePoints(
        TeamBasePoints_t black_base_points,
        TeamPositionPoints_t black_offsets);
    PiecePoints(
        TeamBasePoints_t black_base_points,
        TeamPositionPoints_t black_offsets,
        TeamBasePoints_t red_base_points,
        TeamPositionPoints_t red_offsets);
    TeamPositionPoints_t black_piece_points_;
    TeamPositionPoints_t red_piece_points_;
};
