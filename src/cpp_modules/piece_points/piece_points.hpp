#ifndef _PIECE_POINTS_
#define _PIECE_POINTS_

// #include <minimax_evaluator.hpp>
#include <any>
#include <board_components.hpp>
#include <common.hpp>
#include <nlohmann/json.hpp>
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

// enum PointsFormat { bpo_spec, net_points };

// PointsFormat identify_points_format(json &json_object);
// GamePointsArray_t build_game_points_array(GamePointsSMap_t s_map);
// GamePointsArray_t build_game_points_array(PointsSpecBPOExternal bpo_spec);

// template <typename T>
// struct DataFromPointsJson {
//   T data;
// };



// const unordered_map<PointsFormat, any> my_map = {
// {PointsFormat::bpo_spec, PointsSpecBPOExternal},
// {PointsFormat::net_points, }
// };

// template <typename T>
// void update_type_map(
//     unordered_map<PointsFormat, any> type_map,
//     PointsFormat input_format,
//     T data_type
// ) {
//   type_map[input_format] = data_type;
// }

// class PointsImporter {
// public:
//   PointsImporter(string points_file);
//   GamePointsArray_t ImportPoints();

// private:
//   json json_object;
//   unordered_map<PointsFormat, any> dispatch_ = {
//       {PointsFormat::bpo_spec, PointsSpecBPOExternal}};
// };


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

// Primary conversion functions
GamePointsArray_t game_points_smap_to_array(GamePointsSMap_t s_map);
GamePointsSMap_t game_points_array_to_smap(GamePointsArray_t game_array);

// Intermediate conversion helpers
GamePointsEMap_t game_points_array_to_emap(GamePointsArray_t game_array);
TeamPointsEMap_t team_array_to_emap(TeamPointsArray_t team_array);
GamePointsSMap_t game_points_emap_to_smap(GamePointsEMap_t e_map);
TeamPointsSMap_t team_array_to_smap(TeamPointsArray_t team_array);
GamePointsSMap_t json_to_smap(const json &j);

GamePointsArray_t raw_file_to_array(string raw_points_file);
GamePointsArray_t bpo_file_to_array(string bpo_spec_file);

} // namespace piece_points

#endif // _PIECE_POINTS