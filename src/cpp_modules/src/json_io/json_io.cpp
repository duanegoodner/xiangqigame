#include <json_io.hpp>

using namespace json_io;

GamePointsSMap_t NlohmannJsonIO::Import(string file_path) {
  GamePointsSMap_t game_points{};
  ImportWithTemplate<GamePointsSMap_t>(file_path, game_points);
  return game_points;
}

void NlohmannJsonIO::Export(GamePointsSMap_t &data, string file_path) {
  ExportWithTemplate(data, file_path);
}

bool NlohmannJsonIO::Validate(string data_file, string schema_file) {
  nlohmann::json_schema::json_validator validator;

  try {
    ifstream schema_input(schema_file);
    nlohmann::json schema_json = nlohmann::json::parse(schema_input);
    validator.set_root_schema(schema_json);
  } catch (const exception &e) {
    cerr << "Unable to generate schema validator from provided schema path: "
            "blah blah blah"
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
