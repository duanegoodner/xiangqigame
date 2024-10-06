#pragma once

#include <fstream>
#include <json_utility_interface.hpp>
// #include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>
// #include <nlohmann/json_fwd.hpp>
#include <piece_points_spec.hpp>
#include <string>

using namespace std;

namespace jsonio {
class NlohmannJsonUtility : public JsonUtility<NlohmannJsonUtility> {
public:
  template <typename T>
  void ImplementImport(T &object, const string file_path) {
    nlohmann::json j = ImportToSerializedJson(file_path);
    object = j.get<T>();
  };

  template <typename T>
  void ImplementExport(T &object, const string file_path) {
    nlohmann::json j;
    j = object;
    ExportSerializedJson(j, file_path);
  };

  void ImplementImport(
      piece_points::BPOPointsSKeys &bpo_points,
      const string file_path
  ) {
    ifstream input{file_path};
    auto serialized_json = nlohmann::json::parse(input);
    serialized_json.at("black_base").get_to(bpo_points.black_base_);
    serialized_json.at("red_base_offsets").get_to(bpo_points.red_base_offsets_);
    serialized_json.at("black_position").get_to(bpo_points.black_position_);
    serialized_json.at("red_position_offsets").get_to(bpo_points.red_position_offsets_);
  }

  void ImplementExport(
      piece_points::BPOPointsSKeys &bpo_points,
      const string file_path
  ) {
    auto serialized_json = Serialize(bpo_points);
    ExportSerializedJson(serialized_json, file_path);
  }

private:
  void ExportSerializedJson(nlohmann::json j, const string file_path) {
    ofstream fout{file_path};
    fout << setw(4) << j << endl;
  }

  nlohmann::json ImportToSerializedJson(const string file_path) {
    ifstream input{file_path};
    return nlohmann::json::parse(input);
  }

  nlohmann::json Serialize(piece_points::BPOPointsSKeys &bpo_points) {
    nlohmann::json j;
    j["black_base"] = bpo_points.black_base_;
    j["red_base_offsets"] = bpo_points.red_base_offsets_;
    j["black_position"] = bpo_points.black_position_;
    j["red_position_offsets"] = bpo_points.red_position_offsets_;
    return j;
  }
};
} // namespace jsonio
