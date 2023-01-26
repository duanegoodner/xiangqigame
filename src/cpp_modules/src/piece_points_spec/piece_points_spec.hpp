#ifndef E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802
#define E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802

#include <common.hpp>
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace piece_points_spec {
using namespace std;
using nloh_json = nlohmann::json;

template <typename JsonType>
struct TeamPoints {
  TeamPoints() = default;
  TeamPoints(JsonType &j);
  unordered_map<string, PiecePointsArray_t> PiecePointsArrays();
  JsonType ToJson();
  TeamPointsArray_t ToArray();

private:
  PiecePointsArray_t null;
  PiecePointsArray_t general;
  PiecePointsArray_t advisor;
  PiecePointsArray_t elephant;
  PiecePointsArray_t horse;
  PiecePointsArray_t chariot;
  PiecePointsArray_t cannon;
  PiecePointsArray_t soldier;
};

template <typename JsonType>
struct GamePoints {
  GamePoints() = default;
  // GamePoints(
  //     TeamPoints<JsonType> black_team_points,
  //     TeamPoints<JsonType> red_team_points
  // );
  GamePoints(JsonType &j);



  unordered_map<string, TeamPoints<JsonType>> TeamPointsJsons();
  JsonType ToJson();
  void FromJson();
  GamePointsArray_t ToArray();

private:
  TeamPoints<JsonType> red;
  TeamPoints<JsonType> black;
};

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
  PointsSpecBPOExternal(const nloh_json &json_object);
  PointsSpecBPOExternal(string json_file_path);

  base_points_map_t black_base;
  base_points_map_t red_base_offsets;
  TeamPointsSMap_t black_position;
  TeamPointsSMap_t red_position_offsets;

  nloh_json ToJson();
  void ToFile(string output_path);
  GamePointsSMap_t ToGamePointsSmap();
  GamePointsArray_t ToGamePointsArray();
};

struct RawPoints {
  RawPoints(const nloh_json &json_object);
  RawPoints(string json_file);

  TeamPointsSMap_t black_points;
  TeamPointsSMap_t red_points;

  nloh_json ToJson();
  void ToFile();
  GamePointsArray_t ToGamePointsArray();
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




} // namespace piece_points_spec

#include <piece_points_spec.tpp>


#endif /* E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802 */
