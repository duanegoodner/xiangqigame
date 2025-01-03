//! @file json_utility_nlohmann.cpp
//! Implementation of methods for jsonio::NlohmannJsonUtility.

#include <fstream>
#include <iostream>
#include <jsonio/json_utility_nlohmann.hpp>
#include <piecepoints/base_position_offset.hpp>

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

nlohmann::json jsonio::NlohmannJsonUtility::Serialize(
    piecepoints::BPOPointsSKeys &bpo_points
) {
  nlohmann::json j;
  j["black_base"] = bpo_points.black_base_;
  j["red_base_offsets"] = bpo_points.red_base_offsets_;
  j["black_position"] = bpo_points.black_position_;
  j["red_position_offsets"] = bpo_points.red_position_offsets_;
  return j;
}

