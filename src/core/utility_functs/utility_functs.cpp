// Filename: utility_functs.cpp
// Author: Duane Goodner
// Created: 2022-12-07
// Last Modified: 2024-08-16

// Description:
// Contains implementations of the utility functions that are not defined inline in .hpp file.

#include <fstream>
#include <iostream>
#include <string>
#include <utility_functs.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>


using namespace std;
using nloh_json = nlohmann::json;
using nlohmann::json_schema::json_validator;

nloh_json utility_functs::import_json(string file_path) {
  ifstream input(file_path);
  return json::parse(input);
}

const string utility_functs::get_data_file_abs_path(const string data_file) {
  const string abs_path = string(DATA_DIR) + "/" + data_file;
  return abs_path;
}

bool utility_functs::json_matches_schema(const json &imported_data, const json &schema) {
  json_validator validator;

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
