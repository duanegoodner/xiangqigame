#include "piece_points.hpp"

using namespace std;

BasePts::BasePts(BasePoints_t base_point_vals)
    : base_point_vals_{base_point_vals} {};

PositionPts::PositionPts(AllPositionPts_t pts_arrays_black_) 
    : pts_arrays_black_{pts_arrays_black_} {};
