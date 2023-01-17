#ifndef _PIECE_POINTS_
#define _PIECE_POINTS_

// #include <minimax_evaluator.hpp>
#include <board_components.hpp>
#include <common.hpp>
#include <json.hpp>
#include <minimax_evaluator.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace piece_points {

using namespace board_components;
using namespace std;
using json = nlohmann::json;

// Piece Points spec in "Base Points Offset" form with string keys to easily
// read/write external json
struct PointsSpecBPOExternal {
  PointsSpecBPOExternal(
      base_points_map_t black_base_input,
      base_points_map_t red_base_offsets_input,
      TeamPointsSMap_t black_position_input,
      TeamPointsSMap_t red_position_offsets_input
  );
  PointsSpecBPOExternal(const json &json_object);
  PointsSpecBPOExternal(string json_file_path);
  base_points_map_t black_base;
  base_points_map_t red_base_offsets;
  TeamPointsSMap_t black_position;
  TeamPointsSMap_t red_position_offsets;
  json ToJson();
  void ToFile(string output_path);
};

// Piece Points spec in "Base Points Offset" form with PieceType enum keys for
// use in internal data structs
struct PointsSpecBPOInternal {
  PointsSpecBPOInternal(
      TeamBasePoints_t black_base_input,
      TeamBasePoints_t red_base_offsets_input,
      TeamPointsEMap_t black_position_input,
      TeamPointsEMap_t red_position_offsets_input
  );
  PointsSpecBPOInternal(PointsSpecBPOExternal external_spec);

  TeamBasePoints_t black_base;
  TeamBasePoints_t red_base_offsets;
  TeamPointsEMap_t black_position;
  TeamPointsEMap_t red_position_offsets;
};

class PiecePointsBuilder {
public:
  PiecePointsBuilder(PointsSpecBPOInternal internal_points_spec);
  PiecePointsBuilder(PointsSpecBPOExternal external_points_spec);
  PiecePointsBuilder(string spec_file_path);

  GamePointsEMap_t BuildGamePoints();

private:
  PointsSpecBPOInternal points_spec_;
  PiecePositionPoints_t FlipBoardDirection(PiecePositionPoints_t orig_piece_pts
  );
  PiecePositionPoints_t PiecePointsArraySum(
      PiecePositionPoints_t a,
      PiecePositionPoints_t b
  );
  TeamPointsEMap_t ComputeBlackNetPoints();
  TeamPointsEMap_t ComputeRedNetPoints();
  PiecePositionPoints_t ComputePieceNetPoints(
      Points_t base,
      PiecePositionPoints_t position_points
  );
};

class GamePointsArrayBuilder {
public:
  GamePointsArrayBuilder(PointsSpecBPOInternal internal_points_spec);
  GamePointsArrayBuilder(PointsSpecBPOExternal external_points_spec);
  GamePointsArrayBuilder(string spec_file_path);
  GamePointsArray_t BuildGamePointsArray();

private:
  PointsSpecBPOInternal points_spec_;
  PiecePositionPoints_t FlipBoardDirection(PiecePositionPoints_t orig_piece_pts
  );
  PiecePositionPoints_t PiecePointsArraySum(
      PiecePositionPoints_t a,
      PiecePositionPoints_t b
  );
  PiecePositionPoints_t ComputePieceNetPoints(
      Points_t base,
      PiecePositionPoints_t position_points
  );
  TeamPointsArray_t ComputeBlackNetPoints();
  TeamPointsArray_t ComputeRedNetPoints();
};

const string kICGAPath =
    "/home/duane/workspace/project/src/cpp_modules/piece_points/"
    "ICGA_2004_bpo.json";
const auto DEFAULT_GAME_POINTS =
    PiecePointsBuilder(kICGAPath).BuildGamePoints();

const auto DEFAULT_GAME_POINTS_ARRAY =
    GamePointsArrayBuilder(kICGAPath).BuildGamePointsArray();

struct GamePoints : public PieceValueProvider<GamePoints>{
  GamePoints();
  GamePoints(GamePointsArray_t game_points_array);
  GamePoints(PointsSpecBPOInternal internal_bpo_spec);
  GamePoints(PointsSpecBPOExternal external_bpo_spec);
  GamePoints(string raw_points_json);
  
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