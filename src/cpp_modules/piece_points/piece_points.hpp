#ifndef _PIECE_POINTS_
#define _PIECE_POINTS_

#include <board_components.hpp>
#include <common.hpp>
#include <json.hpp>
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
      position_points_map_t black_position_input,
      position_points_map_t red_position_offsets_input
  );
  PointsSpecBPOExternal(const json &json_object);
  PointsSpecBPOExternal(string json_file_path);
  base_points_map_t black_base;
  base_points_map_t red_base_offsets;
  position_points_map_t black_position;
  position_points_map_t red_position_offsets;
  json ToJson();
  void ToFile(string output_path);
};


// Piece Points spec in "Base Points Offset" form with PieceType enum keys for
// use in internal data structs
struct PointsSpecBPOInternal {
  PointsSpecBPOInternal(
      TeamBasePoints_t black_base_input,
      TeamBasePoints_t red_base_offsets_input,
      TeamPositionPoints_t black_position_input,
      TeamPositionPoints_t red_position_offsets_input
  );
  PointsSpecBPOInternal(PointsSpecBPOExternal external_spec);

  TeamBasePoints_t black_base;
  TeamBasePoints_t red_base_offsets;
  TeamPositionPoints_t black_position;
  TeamPositionPoints_t red_position_offsets;
};

class PiecePointsBuilder {
public:
  PiecePointsBuilder(PointsSpecBPOInternal internal_points_spec);
  PiecePointsBuilder(PointsSpecBPOExternal external_points_spec);
  PiecePointsBuilder(string spec_file_path);

  GamePositionPoints_t BuildGamePositionPoints();

private:
  PointsSpecBPOInternal points_spec_;
  PiecePositionPoints_t FlipBoardDirection(PiecePositionPoints_t orig_piece_pts
  );
  PiecePositionPoints_t PiecePointsArraySum(
      PiecePositionPoints_t a,
      PiecePositionPoints_t b
  );
  TeamPositionPoints_t ComputeBlackNetPoints();
  TeamPositionPoints_t ComputeRedNetPoints();
  PiecePositionPoints_t ComputePieceNetPoints(
      Points_t base,
      PiecePositionPoints_t position_points
  );
};

const string kICGAPath =
    "/home/duane/workspace/project/src/cpp_modules/piece_points/"
    "ICGA_2004_bpo.json";
const auto DEFAULT_GAME_POINTS =
    PiecePointsBuilder(kICGAPath).BuildGamePositionPoints();

} // namespace piece_points

#endif // _PIECE_POINTS