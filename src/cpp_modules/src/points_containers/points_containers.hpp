#ifndef ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7
#define ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7

#include <common.hpp>
#include <unordered_map>

namespace points_containers {

struct TeamPoints {
  TeamPoints() = default;
  TeamPoints(TeamPointsSMap_t s_map);
  unordered_map<string, PiecePointsArray_t> PiecePointsArrays();
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
  GamePoints(GamePointsSMap_t s_map);
  GamePoints(string file_path);

  unordered_map<string, TeamPoints> TeamPointsStructs();
  GamePointsArray_t ToArray();
  void ToFile(string file_path);

  TeamPoints red;
  TeamPoints black;
};

} // namespace points_containers

#endif /* ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7 */
