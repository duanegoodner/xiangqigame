#ifndef E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802
#define E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802

#include <common.hpp>
#include <nlohmann/json.hpp>

namespace piece_points_spec {
using namespace std;
using json = nlohmann::json;

// Piece Points spec in "Base Points Offset" form with string keys to easily
// read/write external json
struct PointsSpecBPOExternal {
  PointsSpecBPOExternal() = default;
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

// class PiecePointsBuilder {
// public:
//   PiecePointsBuilder(PointsSpecBPOInternal internal_points_spec);
//   PiecePointsBuilder(PointsSpecBPOExternal external_points_spec);
//   PiecePointsBuilder(string spec_file_path);

//   GamePointsEMap_t BuildGamePoints();

// private:
//   PointsSpecBPOInternal points_spec_;
//   PiecePositionPoints_t FlipBoardDirection(PiecePositionPoints_t orig_piece_pts
//   );
//   PiecePositionPoints_t PiecePointsArraySum(
//       PiecePositionPoints_t a,
//       PiecePositionPoints_t b
//   );
//   TeamPointsEMap_t ComputeBlackNetPoints();
//   TeamPointsEMap_t ComputeRedNetPoints();
//   PiecePositionPoints_t ComputePieceNetPoints(
//       Points_t base,
//       PiecePositionPoints_t position_points
//   );
// };

}

#endif /* E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802 */
