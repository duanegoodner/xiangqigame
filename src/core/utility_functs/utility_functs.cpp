// Filename: utility_functs.cpp
// Author: Duane Goodner
// Created: 2022-12-07
// Last Modified: 2024-08-16

// Description:
// Contains implementations of the utility functions that are not defined inline in .hpp file.

#include <fstream>
#include <string>
#include <utility_functs.hpp>
#include <nlohmann/json.hpp>


using namespace std;
using nloh_json = nlohmann::json;

nloh_json utility_functs::import_json(string file_path) {
  ifstream input(file_path);
  return json::parse(input);
}

void utility_functs::export_json(const nloh_json &j, string filename) {
  ofstream fout(filename);
  fout << setw(4) << j << endl;
}

const string utility_functs::get_data_file_abs_path(const string data_file) {
  const string abs_path = string(DATA_DIR) + "/" + data_file;
  return abs_path;
}
