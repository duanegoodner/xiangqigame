//! @file piecepoints/base_position_offset.hpp
//! Definitions of classes used for storing piece points in Base Points Offset form.

#pragma once

#include <json_utility_interface.hpp>
#include <memory>
#include <json_utility_nlohmann.hpp>
#include <piecepoints/piece_points_data_structs.hpp>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;

namespace jsonio {
class NlohmannJsonUtility;
}

namespace piecepoints {

// forward declare BPOPointsEKeys so BPOPointsSKeys is aware of it
class BPOPointsEKeys;

//! Piece Points spec in "Base Points Offset" form with string keys in member
//! unordered_map objects for easy reading / writing data from / to .json files.
class BPOPointsSKeys {
public:
  BPOPointsSKeys(const BPOPointsSKeys &) = delete;
  BPOPointsSKeys &operator=(const BPOPointsSKeys &) = delete;

  BPOPointsSKeys();
  BPOPointsSKeys(
      BasePointsSMap_t black_base_input,
      BasePointsSMap_t red_base_offsets_input,
      TeamPointsSMap_t black_position_input,
      TeamPointsSMap_t red_position_offsets_input
  );
  BPOPointsSKeys(const string &json_file_path);

  BasePointsSMap_t black_base_;
  BasePointsSMap_t red_base_offsets_;
  TeamPointsSMap_t black_position_;
  TeamPointsSMap_t red_position_offsets_;

  void ToFile(string output_path);
  BPOPointsEKeys ToBPOPointsEKeys();
  GamePointsSMap_t ToGamePointsSmap();
  GamePointsArray_t ToGamePointsArray();

private:
  unique_ptr<JsonUtility<jsonio::NlohmannJsonUtility>> json_utility_;
};

//! Piece Points spec in "Base Points Offset" form with PieceType enum keys in member
//! unordered_map objects for easy conversion to / from piecepoints::GamePointsArray_t
//! form.
class BPOPointsEKeys {
public:
  BPOPointsEKeys(
      TeamBasePoints_t black_base_input,
      TeamBasePoints_t red_base_offsets_input,
      TeamPointsEMap_t black_position_input,
      TeamPointsEMap_t red_position_offsets_input
  );
  BPOPointsEKeys(BPOPointsSKeys external_spec);
  TeamPointsArray_t BlackNetPoints();
  TeamPointsArray_t RedNetPoints();
  GamePointsArray_t ToGamePointsArray();

  TeamBasePoints_t black_base_;
  TeamBasePoints_t red_base_offsets_;
  TeamPointsEMap_t black_position_;
  TeamPointsEMap_t red_position_offsets_;
};

const string kICGABPOPath = utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
const string kICGARawPath = utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
const string kRawSchemaPath =
    utility_functs::get_data_file_abs_path("raw_points_schema.json");
const string kBPOSchemaPath = utility_functs::get_data_file_abs_path("bpo_schema.json");
} // namespace piecepoints
