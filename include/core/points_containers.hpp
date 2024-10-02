// Filename: points_containers.hpp
// Author: Duane Goodner
// Created: 2022-12-15
// Last Modified: 2024-08-16

// Description:
// Definitions of structs that hold piece points data.

#ifndef ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7
#define ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7

#include <common.hpp>
#include <unordered_map>
#include <points_containers_details.hpp>

namespace points_containers {

struct GamePoints {
  GamePoints() = default;
  GamePoints(TeamPoints black_points, TeamPoints red_points);
  GamePoints(GamePointsSMap_t s_map);
  GamePoints(GamePointsArray_t game_point_array);

  TeamPoints red;
  TeamPoints black;

  GamePointsArray_t ToArray();
  GamePointsSMap_t ToMap();
};

struct BasePointsOffsetSpec {
  BasePointsOffsetSpec() = default;
  BasePointsOffsetSpec(BPOSpecSMap_t s_map);

  TeamBasePositionPair black;
  TeamBasePositionPair red;

  // PieceBasePoints black_base;
  // TeamPoints black_position;
  // PieceBasePoints red_base_offsets;
  // TeamPoints red_position_offsets;

  GamePoints ToGamePoints();
  BPOSpecSMap_t ToMap();

private:
  TeamPoints BlackNetPoints();
  TeamPoints RedNetPoints();
};

} // namespace points_containers

#endif /* ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7 */
