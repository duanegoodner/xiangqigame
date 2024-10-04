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

class PointsSpecBPOExternal;

class BPOFileHandler {
public:
  virtual ~BPOFileHandler() = default;
  virtual void Import(PointsSpecBPOExternal &bpo_points, string file_path) = 0;
  virtual void Export(PointsSpecBPOExternal &bpo_points, string file_path) = 0;
};

class NlohmannBPOFileHandler : public BPOFileHandler {
public:
  void Import(PointsSpecBPOExternal &bpo_points, string file_path);
  void Export(PointsSpecBPOExternal &bpo_points, string file_path);

private:
  nloh_json ToJsonObject(PointsSpecBPOExternal &bpo_points);
};

// Piece Points spec in "Base Points Offset" form with string keys to easily
// read/write external json
struct PointsSpecBPOExternal {
  PointsSpecBPOExternal();
  PointsSpecBPOExternal(
      BasePointsSMap_t black_base_input,
      BasePointsSMap_t red_base_offsets_input,
      TeamPointsSMap_t black_position_input,
      TeamPointsSMap_t red_position_offsets_input
  );
  PointsSpecBPOExternal(const string &json_file_path);

  BasePointsSMap_t black_base;
  BasePointsSMap_t red_base_offsets;
  TeamPointsSMap_t black_position;
  TeamPointsSMap_t red_position_offsets;

  void ToFile(string output_path);
  GamePointsSMap_t ToGamePointsSmap();
  GamePointsArray_t ToGamePointsArray();

private:
  NlohmannBPOFileHandler file_handler;
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

#endif /* E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802 */
