#ifndef AA3D35D0_D202_4308_AFB7_5E99E3F7DF89
#define AA3D35D0_D202_4308_AFB7_5E99E3F7DF89

#include <string>
#include <utility_functs.hpp>

using namespace std;
using json = nlohmann::json;

const string kICGABPOPath = utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
const string kICGARawPath = utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
const string kRawSchemaPath =
    utility_functs::get_data_file_abs_path("raw_points_schema.json");
const string kBPOSchemaPath = utility_functs::get_data_file_abs_path("bpo_schema.json");


#endif /* AA3D35D0_D202_4308_AFB7_5E99E3F7DF89 */
