
#include "rapidjson/rapidjson.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/schema.h>
#include <string>
#include <unordered_map>
#include <utility_functs.hpp>
#include <vector>

using json = nlohmann::json;

rapidjson::Document import_json(std::string json_file) {
  std::ifstream is(json_file);
  rapidjson::IStreamWrapper is_wrapper(is);
  rapidjson::Document d;
  d.ParseStream(is_wrapper);

  return d;
}



int main() {

  std::string raw_pts_path =
      "/home/duane/workspace/project/src/cpp_modules/data/"
      "ICGA_2004_raw.json";

  std::string bpo_pts_path =
      "/home/duane/workspace/project/src/cpp_modules/data/"
      "ICGA_2004_bpo.json";

  std::string raw_schema_path =
      "/home/duane/workspace/project/src/cpp_modules/data/"
      "raw_points_schema.json";

  std::string bpo_schema_path =
      "/home/duane/workspace/project/src/cpp_modules/data/"
      "bpo_schema.json";

  std::ifstream input(raw_pts_path);
  rapidjson::IStreamWrapper isw(input);

    rapidjson::Document d;
  d.ParseStream(isw);

  for (rapidjson::Value::ConstMemberIterator itr = d.MemberBegin();
       itr != d.MemberEnd();
       ++itr) {
    std::cout << itr->name.GetString() << std::endl;
    for (rapidjson::Value::ConstMemberIterator inner_itr =
             itr->value.MemberBegin();
         inner_itr != itr->value.MemberEnd();
         ++inner_itr) {
            std::cout << inner_itr->name.GetString() << std::endl;
            rapidjson::Value& cur_array = d[itr->name.GetString()][inner_itr->name.GetString()];
            for (rapidjson::SizeType rank = 0; rank < 10; ++rank ) {
              for (rapidjson::SizeType file = 0; file < 9; ++file) {
                std::cout << cur_array[rank][file].GetInt() << " ";
              }
              std::cout << std::endl;
            }
    }
    std::cout << std::endl;
  }

  auto raw_schema_json = import_json(raw_schema_path);
  auto bpo_schema_json = import_json(bpo_schema_path);
  rapidjson::SchemaDocument raw_schema(raw_schema_json);
  rapidjson::SchemaDocument bpo_schema(bpo_schema_json);

  auto raw_input = import_json(raw_pts_path);
  auto bpo_input = import_json(bpo_pts_path);
  
  std::unordered_map<std::string, rapidjson::SchemaDocument *> my_schemas{};
  my_schemas.insert({"raw", &raw_schema});
  my_schemas.insert({"bpo", &bpo_schema});

  rapidjson::SchemaValidator raw_validator(raw_schema);
  rapidjson::SchemaValidator bpo_validator(bpo_schema);

  auto raw_result = raw_input.Accept(raw_validator);
  auto bpo_result = bpo_input.Accept(bpo_validator);

  // auto nloh_raw_schema = utility_functs::import_json(raw_schema_path);
  // auto nloh_bpo_schema = utility_functs::import_json(bpo_schema_path);
  // std::unordered_map<std::string, json &> nloh_schemas{};
  // nloh_schemas.insert({"raw", nloh_raw_schema});
  // nloh_schemas.insert({"bpo", nloh_bpo_schema});

  return 0;
}