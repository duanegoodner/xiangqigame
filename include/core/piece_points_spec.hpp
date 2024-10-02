// Filename: piece_points_spec.hpp
// Author: Duane Goodner
// Created: 2022-12-15
// Last Modified: 2024-08-16

// Description:
// Defines classes that enforce specs that external json files with piece
// points info must comply with.

#ifndef E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802
#define E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802

#include <common.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility_functs.hpp>

namespace piece_points {
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

struct PieceBasePoints {
  int advisor;
  int cannon;
  int chariot;
  int elephant;
  int general;
  int horse;
  int null;
  int soldier;
};

template <typename JsonType>
struct BasePointOffsetSpec {
  BasePointOffsetSpec() = default;
  BasePointOffsetSpec(JsonType &j);
  BasePointOffsetSpec(string file_path);

  PieceBasePoints black_base;
  TeamPoints<JsonType> black_position;
  PieceBasePoints red_base_offsets;
  TeamPoints<JsonType> red_position_offsets;
};


// Piece Points spec in "Base Points Offset" form with string keys to easily
// read/write external json
struct PointsSpecBPOExternal {
  PointsSpecBPOExternal() = default;
  PointsSpecBPOExternal(
      BasePointsSMap_t black_base_input,
      BasePointsSMap_t red_base_offsets_input,
      TeamPointsSMap_t black_position_input,
      TeamPointsSMap_t red_position_offsets_input
  );
  PointsSpecBPOExternal(const nloh_json &json_object);
  PointsSpecBPOExternal(string json_file_path);

  BasePointsSMap_t black_base;
  BasePointsSMap_t red_base_offsets;
  TeamPointsSMap_t black_position;
  TeamPointsSMap_t red_position_offsets;

  nloh_json ToJson();
  void ToFile(string output_path);
  GamePointsSMap_t ToGamePointsSmap();
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

const string kICGABPOPath_x =
    utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
const string kICGARawPath_x =
    utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
const string kRawSchemaPath_x =
    utility_functs::get_data_file_abs_path("raw_points_schema.json");
const string kBPOSchemaPath_x =
    utility_functs::get_data_file_abs_path("bpo_schema.json");
} // namespace piece_points

// #include <piece_points_spec.tpp>

#endif /* E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802 */
