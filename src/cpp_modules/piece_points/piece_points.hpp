#ifndef _PIECE_POINTS_
#define _PIECE_POINTS_

// #include <minimax_evaluator.hpp>
#include <board_components.hpp>
#include <common.hpp>
#include <json.hpp>
#include <minimax_evaluator.hpp>
#include <piece_points_spec.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace piece_points {

using namespace board_components;
using namespace piece_points_spec;
using namespace std;
using json = nlohmann::json;

class GamePointsArrayBuilder {
public:
  GamePointsArrayBuilder(PointsSpecBPOInternal internal_points_spec);
  GamePointsArrayBuilder(PointsSpecBPOExternal external_points_spec);
  GamePointsArrayBuilder(string spec_file_path);
  GamePointsArray_t BuildGamePointsArray();

private:
  PointsSpecBPOInternal points_spec_;
  TeamPointsArray_t ComputeBlackNetPoints();
  TeamPointsArray_t ComputeRedNetPoints();
};

const string kICGAPath =
    "/home/duane/workspace/project/src/cpp_modules/piece_points/"
    "ICGA_2004_bpo.json";

const auto DEFAULT_GAME_POINTS_ARRAY =
    GamePointsArrayBuilder(kICGAPath).BuildGamePointsArray();

struct PiecePoints : public PieceValueProvider<PiecePoints> {
  PiecePoints();
  PiecePoints(GamePointsArray_t game_points_array);
  PiecePoints(PointsSpecBPOInternal internal_bpo_spec);
  PiecePoints(PointsSpecBPOExternal external_bpo_spec);
  PiecePoints(string raw_points_json);

  // define in header to force inlining
  Points_t ImplementGetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return points_array[get_zcolor_index(color)][piece_type][space.rank]
                       [space.file];
  }

  GamePointsArray_t points_array;
  void ToJson();
  void ToFile();
};

// Primary conversion functions
GamePointsArray_t game_points_smap_to_array(GamePointsSMap_t s_map);
GamePointsSMap_t game_points_array_to_smap(GamePointsArray_t game_array);

// Intermediate conversion helpers
GamePointsEMap_t game_points_array_to_map(GamePointsArray_t game_array);
TeamPointsEMap_t team_array_to_map(TeamPointsArray_t team_array);
GamePointsSMap_t game_points_emap_to_smap(GamePointsEMap_t e_map);
TeamPointsSMap_t team_array_to_smap(TeamPointsArray_t team_array);

} // namespace piece_points

#endif // _PIECE_POINTS