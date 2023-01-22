#ifndef _PIECE_POINTS_
#define _PIECE_POINTS_

// #include <minimax_evaluator.hpp>
#include "utility_functs.hpp"
#include <any>
#include <board_components.hpp>
#include <common.hpp>
#include <functional>
#include <minimax_evaluator.hpp>
#include <nlohmann/json.hpp>
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

const string kICGABPOPath =
    "/home/duane/workspace/project/src/cpp_modules/src/piece_points/"
    "ICGA_2004_bpo.json";

const string kICGARawPath =
    "/home/duane/workspace/project/src/cpp_modules/src/piece_points/"
    "ICGA_2004_raw.json";

const string kRawSchemaPath =
    "/home/duane/workspace/project/src/cpp_modules/src/piece_points/"
    "raw_points_schema.json";

const string kBPOSchemaPath =
    "/home/duane/workspace/project/src/cpp_modules/src/piece_points/"
    "bpo_schema.json";

const auto DEFAULT_GAME_POINTS_ARRAY =
    GamePointsArrayBuilder(kICGABPOPath).BuildGamePointsArray();

// Primary conversion functions
GamePointsArray_t game_points_smap_to_array(GamePointsSMap_t s_map);
GamePointsSMap_t game_points_array_to_smap(GamePointsArray_t game_array);

GamePointsSMap_t raw_points_to_smap(const json &raw_points);
GamePointsSMap_t points_spec_to_smap(const json &points_spec);

// Intermediate conversion helpers
GamePointsEMap_t game_points_array_to_emap(GamePointsArray_t game_array);
TeamPointsEMap_t team_array_to_emap(TeamPointsArray_t team_array);
GamePointsSMap_t game_points_emap_to_smap(GamePointsEMap_t e_map);
TeamPointsSMap_t team_array_to_smap(TeamPointsArray_t team_array);
// GamePointsSMap_t raw_points_to_smap(const json &j);

enum PointsImportFormat { raw, bpo, invalid };

typedef unordered_map<PointsImportFormat, json> pts_format_to_schema_t;
typedef unordered_map<PointsImportFormat, string> pts_format_to_schema_path_t;
inline pts_format_to_schema_t import_schemas(pts_format_to_schema_path_t schema_paths
) {
  pts_format_to_schema_t schemas;
  for (auto schema_path : schema_paths) {
    schemas[schema_path.first] =
        utility_functs::import_json(schema_path.second);
  }
  return schemas;
}

const pts_format_to_schema_path_t DEFAULT_POINTS_SCHEMA_PATHS = {
    {PointsImportFormat::raw, kRawSchemaPath},
    {PointsImportFormat::bpo, kBPOSchemaPath}};

const unordered_map<
    PointsImportFormat,
    function<GamePointsSMap_t(const json &)>>
    raw_points_to_smap_dispatch = {
        {PointsImportFormat::raw, raw_points_to_smap},
        {PointsImportFormat::bpo, points_spec_to_smap}};

class FormatIdentifier {
public:
  FormatIdentifier(json json_object, pts_format_to_schema_t schemas);
  FormatIdentifier(json json_object);
  PointsImportFormat Identify();

private:
  json json_object_;
  pts_format_to_schema_t schemas_;
};

class GamePointsArrayBuilder_2 {
public:
  GamePointsArrayBuilder_2(json json_object);
  GamePointsArrayBuilder_2(string json_file_path);
  GamePointsArray_t Build();

private:
  json json_object_;
};

struct PiecePoints : public PieceValueProvider<PiecePoints> {
  PiecePoints();
  PiecePoints(GamePointsArray_t game_points_array);
  PiecePoints(GamePointsSMap_t s_map);
  PiecePoints(json &j);
  PiecePoints(PointsSpecBPOInternal internal_bpo_spec);
  PiecePoints(PointsSpecBPOExternal external_bpo_spec);
  PiecePoints(string json_file);

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
  json ToJson();
  void ToFile(string output_file);
};

bool json_matches_schema(
    const json &json_object,
    const json &schema);

} // namespace piece_points

#endif // _PIECE_POINTS