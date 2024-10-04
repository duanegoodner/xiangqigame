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

class BPOPointsSKeys;
class BPOPointsEKeys;

class BPOFileHandler {
public:
  virtual ~BPOFileHandler() = default;
  virtual void Import(BPOPointsSKeys &bpo_points, string file_path) = 0;
  virtual void Export(BPOPointsSKeys &bpo_points, string file_path) = 0;
};

class AbstractBPOFileHandlerFactory {
public:
  virtual ~AbstractBPOFileHandlerFactory() = default;
  virtual unique_ptr<BPOFileHandler> create_bpo_file_handler() const = 0;
};

class NlohmannBPOFileHandler : public BPOFileHandler {
public:
  void Import(BPOPointsSKeys &bpo_points, string file_path);
  void Export(BPOPointsSKeys &bpo_points, string file_path);

private:
  nloh_json ToJsonObject(BPOPointsSKeys &bpo_points);
};

class NlohmannBPOFileHandlerFactory : public AbstractBPOFileHandlerFactory {
public:
  unique_ptr<BPOFileHandler> create_bpo_file_handler() const override {
    return make_unique<NlohmannBPOFileHandler>();
  }
};

// Piece Points spec in "Base Points Offset" form with string keys to easily
// read/write external json
class BPOPointsSKeys {
public:
  BPOPointsSKeys(const BPOPointsSKeys &) = delete;
  BPOPointsSKeys &operator=(const BPOPointsSKeys &) = delete;

  explicit BPOPointsSKeys(
      const AbstractBPOFileHandlerFactory &file_handler_factory = NlohmannBPOFileHandlerFactory()
  );
  BPOPointsSKeys(
      BasePointsSMap_t black_base_input,
      BasePointsSMap_t red_base_offsets_input,
      TeamPointsSMap_t black_position_input,
      TeamPointsSMap_t red_position_offsets_input,
      const AbstractBPOFileHandlerFactory &file_handler_factory = NlohmannBPOFileHandlerFactory()
  );
  BPOPointsSKeys(
      const string &json_file_path,
      const AbstractBPOFileHandlerFactory &file_handler_factory = NlohmannBPOFileHandlerFactory()
  );

  BasePointsSMap_t black_base_;
  BasePointsSMap_t red_base_offsets_;
  TeamPointsSMap_t black_position_;
  TeamPointsSMap_t red_position_offsets_;

  void ToFile(string output_path);
  BPOPointsEKeys ToBPOPointsEKeys();
  GamePointsSMap_t ToGamePointsSmap();
  GamePointsArray_t ToGamePointsArray();

private:
  unique_ptr<BPOFileHandler> file_handler_;
};

// Piece Points spec in "Base Points Offset" form with PieceType enum keys for
// use in internal data structs
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
