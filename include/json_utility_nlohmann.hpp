//! @file json_utility_nlohmann.hpp
//! Declaration and implementation of templated portions of jsonio::NlohmannJsonUtility.

#pragma once

#include <fstream>
#include <json_utility_interface.hpp>
#include <nlohmann/json.hpp>
#include <piece_points_bpo.hpp>
#include <string>

namespace jsonio {

//! Implements JsonUtility interface; imports / exports objects from / to json files
//! using the [**Nlohmann json library**](https://github.com/nlohmann/json).
class NlohmannJsonUtility : public JsonUtility<NlohmannJsonUtility> {
public:
  template <typename T>
  void ImplementImport(T &object, const std::string file_path) {
    nlohmann::json j = ImportToSerializedJson(file_path);
    object = j.get<T>();
  };

  template <typename T>
  void ImplementExport(T &object, const std::string file_path) {
    nlohmann::json j;
    j = object;
    ExportSerializedJson(j, file_path);
  };

  void ImplementImport(piecepoints::BPOPointsSKeys &bpo_points, const std::string file_path);
  void ImplementExport(piecepoints::BPOPointsSKeys &bpo_points, const std::string file_path);

private:
  void ExportSerializedJson(nlohmann::json serialized_json, const std::string file_path);
  nlohmann::json ImportToSerializedJson(const std::string file_path);
  void Deserialize(
      nlohmann::json serialized_json,
      piecepoints::BPOPointsSKeys &bpo_points
  );
  nlohmann::json Serialize(piecepoints::BPOPointsSKeys &bpo_points);
};

} // namespace jsonio
