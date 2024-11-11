//! @file json_utility_nlohmann.cpp
//! Implementation of methods for jsonio::NlohmannJsonUtility.  

#include <fstream>
#include <iostream>
#include <json_utility_nlohmann.hpp>
#include <piece_points_bpo.hpp>

using namespace std;

void jsonio::NlohmannJsonUtility::ImplementImport(
    piecepoints::BPOPointsSKeys &bpo_points,
    const string file_path
) {
  ifstream input{file_path};
  auto serialized_json = nlohmann::json::parse(input);
  Deserialize(serialized_json, bpo_points);
}

void jsonio::NlohmannJsonUtility::ImplementExport(
    piecepoints::BPOPointsSKeys &bpo_points,
    const string file_path
) {
  auto serialized_json = Serialize(bpo_points);
  ExportSerializedJson(serialized_json, file_path);
}

void jsonio::NlohmannJsonUtility::ExportSerializedJson(
    nlohmann::json serialized_json,
    const string file_path
) {
  ofstream fout{file_path};
  fout << setw(4) << serialized_json << endl;
}

nlohmann::json jsonio::NlohmannJsonUtility::ImportToSerializedJson(const string file_path
) {
  ifstream input{file_path};
  return nlohmann::json::parse(input);
}

void jsonio::NlohmannJsonUtility::Deserialize(
    nlohmann::json serialized_json,
    piecepoints::BPOPointsSKeys &bpo_points
) {
  serialized_json.at("black_base").get_to(bpo_points.black_base_);
  serialized_json.at("red_base_offsets").get_to(bpo_points.red_base_offsets_);
  serialized_json.at("black_position").get_to(bpo_points.black_position_);
  serialized_json.at("red_position_offsets").get_to(bpo_points.red_position_offsets_);
}

nlohmann::json jsonio::NlohmannJsonUtility::Serialize(piecepoints::BPOPointsSKeys &bpo_points) {
    nlohmann::json j;
    j["black_base"] = bpo_points.black_base_;
    j["red_base_offsets"] = bpo_points.red_base_offsets_;
    j["black_position"] = bpo_points.black_position_;
    j["red_position_offsets"] = bpo_points.red_position_offsets_;
    return j;
  }

  nlohmann::json jsonio::import_json(string file_path) {
  ifstream input(file_path);
  return nlohmann::json::parse(input);
}

bool jsonio::json_matches_schema(const nlohmann::json &imported_data, const nlohmann::json &schema) {
  nlohmann::json_schema::json_validator validator;

  try {
    validator.set_root_schema(schema);
  } catch (const exception &e) {
    cerr << "Schema validation failed: " << e.what() << endl;
    exit(1);
  }

  try {
    auto data_validation_result = validator.validate(imported_data);
    return true;
  } catch (const exception &e) {
    return false;
  }
}