// Filename: json_io.hpp
// Author: Duane Goodner
// Created: 2022-12-27
// Last Modified: 2024-08-16

// Description:
// Defines classes for json IO with specific json libraries (As of 2024-08-16,
// only using nlohmann library).

#ifndef A64329FA_6D6E_4026_A7C1_FF253B8769B6
#define A64329FA_6D6E_4026_A7C1_FF253B8769B6

#include "nlohmann/json_fwd.hpp"
#include <any>
#include <common.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <json_interface.hpp>
#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <typeinfo>
#include <unordered_map>

using namespace std;
using namespace json_interface;

namespace json_io {

class NlohmannJsonIO : public JsonInterface {

public:
  void Import(GamePointsSMap_t &s_map, string file_path) override;
  void Export(GamePointsSMap_t &data, string file_path) override;
  void Import(BPOSpecSMap_t &s_map, string file_path) override;
  void Export(BPOSpecSMap_t &data, string file_path) override;

  bool Validate(string data_file, string schema_file) override;

private:
  nlohmann::json ImportToJson(string file_path) {
    ifstream input(file_path);
    return nlohmann::json::parse(input);
  }

  void ExportFromJson(nlohmann::json j, string file_path) {
    ofstream fout(file_path);
    fout << setw(4) << j << endl;
  }

  template <typename T>
  void ImportWithTemplate(string file_path, T &object) {
    // ifstream input(file_path);
    nlohmann::json j = ImportToJson(file_path);
    object = j.get<T>();
    // return object;
  }

  template <typename T>
  void ExportWithTemplate(T &object, string file_path) {
    nlohmann::json j;
    j = object;
    ExportFromJson(j, file_path);
    // ofstream fout(file_path);
    // fout << setw(4) << j << endl;
  }
};
} // namespace json_io

#endif /* A64329FA_6D6E_4026_A7C1_FF253B8769B6 */
