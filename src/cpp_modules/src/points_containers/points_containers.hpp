#ifndef ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7
#define ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7

#include <common.hpp>
#include <points_containers_details.hpp>
#include <unordered_map>
#include <points_containers_details.hpp>

namespace points_containers {

struct TeamPoints {
  TeamPoints() = default;
  TeamPoints(TeamPointsSMap_t s_map);
  TeamPointsArray_t ToArray();

  PiecePointsArray_t null;
  PiecePointsArray_t general;
  PiecePointsArray_t advisor;
  PiecePointsArray_t elephant;
  PiecePointsArray_t horse;
  PiecePointsArray_t chariot;
  PiecePointsArray_t cannon;
  PiecePointsArray_t soldier;
};

struct GamePoints {
  GamePoints() = default;
  GamePoints(TeamPoints black_points, TeamPoints red_points);
  GamePoints(GamePointsSMap_t s_map);
  GamePoints(string file_path);

  unordered_map<string, TeamPoints> TeamPointsStructs();
  GamePointsArray_t ToArray();
  void ToFile(string file_path);

  TeamPoints red;
  TeamPoints black;
};

struct PieceBasePoints {
  PieceBasePoints() = default;
  PieceBasePoints(BasePointsSMap_t base_points);
  
  int advisor;
  int cannon;
  int chariot;
  int elephant;
  int general;
  int horse;
  int null;
  int soldier;
};

struct BasePointsOffsetSpec {
  BasePointsOffsetSpec() = default;

  PieceBasePoints black_base;
  TeamPoints black_position;
  PieceBasePoints red_base_offsets;
  TeamPoints red_position_offsets;

  GamePoints ToGamePoints();

private:
  TeamPointsSMap_t BlackNetSMap();
  TeamPointsSMap_t RedNetSMap(TeamPointsSMap_t black_net_smap);
};

} // namespace points_containers

#endif /* ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7 */
