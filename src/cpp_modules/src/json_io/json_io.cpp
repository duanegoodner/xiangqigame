#include <json_io.hpp>
#include <vector>

using namespace json_io;

void NlohmannJsonIO::Import(GamePointsSMap_t &s_map, string file_path) {
  ImportWithTemplate<GamePointsSMap_t>(file_path, s_map);
}

void NlohmannJsonIO::Export(GamePointsSMap_t &data, string file_path) {
  ExportWithTemplate(data, file_path);
}

void NlohmannJsonIO::Import(BPOSpecSMap_t &s_map, string file_path) {
  auto bpo_spec_json = ImportToJson(file_path);

  BasePointsSMap_t black_base = bpo_spec_json.at("black_base");
  TeamPointsSMap_t black_position = bpo_spec_json.at("black_position");
  BasePointsSMap_t red_base_offsets = bpo_spec_json.at("red_base_offsets");
  TeamPointsSMap_t red_position_offsets =
      bpo_spec_json.at("red_position_offsets");

  BasePositionSMapPair_t black = {black_base, black_position};
  BasePositionSMapPair_t red = {red_base_offsets, red_position_offsets};

  s_map["black"] = black;
  s_map["red"] = red;
}

void NlohmannJsonIO::Export(BPOSpecSMap_t &data, string file_path) {
  nlohmann::json j{};
  j["black_base"] = data.at("black").first;
  j["black_position"] = data.at("black").second;
  j["red_base_offsets"] = data.at("red").first;
  j["red_position_offsets"] = data.at("red").second;
  ExportFromJson(j, file_path);
}

bool NlohmannJsonIO::Validate(string data_file, string schema_file) {
  nlohmann::json_schema::json_validator validator;

  try {
    ifstream schema_input(schema_file);
    nlohmann::json schema_json = nlohmann::json::parse(schema_input);
    validator.set_root_schema(schema_json);
  } catch (const exception &e) {
    cerr << "Unable to generate schema validator from provided schema path: "
         << e.what() << endl;
    exit(1);
  }

  nlohmann::json data_json;
  try {
    ifstream data_input(data_file);
    data_json = nlohmann::json::parse(data_input);
  } catch (const exception &e) {
    cerr << "Unable to import JSON from data file path." << endl
         << "No comparison of data against schema performed." << endl
         << "Current validation function will return 'false'." << endl
         << "Note that the provided data file path would not yield 'true' "
            "with any schema "
            "since data file cannot be imported."
         << endl
         << e.what() << endl;
    return false;
  }

  try {
    validator.validate(data_json);
  } catch (const exception &e) {
    return false;
  }

  return true;
}
