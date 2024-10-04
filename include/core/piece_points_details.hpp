#ifndef AA3D35D0_D202_4308_AFB7_5E99E3F7DF89
#define AA3D35D0_D202_4308_AFB7_5E99E3F7DF89

#include <common.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <utility_functs.hpp>

using namespace std;
using json = nlohmann::json;

const string kICGABPOPath = utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
const string kICGARawPath = utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
const string kRawSchemaPath =
    utility_functs::get_data_file_abs_path("raw_points_schema.json");
const string kBPOSchemaPath = utility_functs::get_data_file_abs_path("bpo_schema.json");

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

#endif /* AA3D35D0_D202_4308_AFB7_5E99E3F7DF89 */
