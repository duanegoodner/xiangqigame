#ifndef E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802
#define E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802

#include <common.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <typeinfo>
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

struct TeamPointsNonTemp {
  TeamPointsNonTemp() = default;
  TeamPointsNonTemp(TeamPointsSMap_t team_points_data);
  // TeamPoints(JsonType &j);
  unordered_map<string, PiecePointsArray_t> PiecePointsArrays();
  // JsonType ToJson();
  TeamPointsArray_t ToArray();

// private:
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
struct BaseOffsetPointSpec {
  BaseOffsetPointSpec() = default;
  BaseOffsetPointSpec(JsonType &j);
  BaseOffsetPointSpec(string file_path);

  PieceBasePoints black_base;
  TeamPoints<JsonType> black_position;
  PieceBasePoints red_base_offsets;
  TeamPoints<JsonType> red_position_offsets;
};

template <typename JsonType>
struct GamePoints {
  GamePoints() = default;
  GamePoints(JsonType &j);
  GamePoints(string file_path);

  unordered_map<string, TeamPoints<JsonType>> TeamPointsJsons();
  JsonType ToJson();
  GamePointsArray_t ToArray();
  void ToFile(string file_path);

private:
  TeamPoints<JsonType> red;
  TeamPoints<JsonType> black;
};

struct GamePointsNonTemp {
  GamePointsNonTemp() = default;
  GamePointsNonTemp(GamePointsSMap_t points_input);
  GamePointsNonTemp(string file_path);

  unordered_map<string, TeamPointsNonTemp> TeamPointsStructs();
  GamePointsArray_t ToArray();
  void ToFile(string file_path);


  TeamPointsNonTemp red;
  TeamPointsNonTemp black;
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
    "/home/duane/workspace/project/src/cpp_modules/data/"
    "ICGA_2004_bpo.json";

const string kICGARawPath_x =
    "/home/duane/workspace/project/src/cpp_modules/data/"
    "ICGA_2004_raw.json";

const string kRawSchemaPath_x =
    "/home/duane/workspace/project/src/cpp_modules/data/"
    "raw_points_schema.json";

const string kBPOSchemaPath_x =
    "/home/duane/workspace/project/src/cpp_modules/data/"
    "bpo_schema.json";


inline const size_t kGamePointsNlohmannJson =
    typeid(GamePoints<nloh_json>).hash_code();
inline const size_t kBPOSpecNlohmannJson =
    typeid(BaseOffsetPointSpec<nloh_json>).hash_code();


template <typename ImportJsonType>
bool game_points_struct_match_json(
    GamePoints<ImportJsonType>& game_points,
    ImportJsonType &j
);
} // namespace piece_points_spec

#include <piece_points_spec.tpp>

#endif /* E0F8CBC1_E4D2_4FE0_9B50_4D7799B44802 */
