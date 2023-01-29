#ifndef A64329FA_6D6E_4026_A7C1_FF253B8769B6
#define A64329FA_6D6E_4026_A7C1_FF253B8769B6

#include "nlohmann/json_fwd.hpp"
#include <any>
#include <common.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>
#include <points_containers.hpp>
#include <string>
#include <typeinfo>
#include <unordered_map>

using namespace std;

namespace points_containers {
void to_json(nlohmann::json &j, const TeamPoints &t) {
  j = nlohmann::json{
      {"null", t.null},
      {"general", t.general},
      {"advisor", t.advisor},
      {"elephant", t.elephant},
      {"horse", t.horse},
      {"chariot", t.chariot},
      {"cannon", t.cannon},
      {"soldier", t.soldier}};
}

void from_json(const nlohmann::json &j, TeamPoints &t) {
  j.at("null").get_to(t.null);
  j.at("general").get_to(t.general);
  j.at("advisor").get_to(t.advisor);
  j.at("elephant").get_to(t.elephant);
  j.at("horse").get_to(t.horse);
  j.at("chariot").get_to(t.chariot);
  j.at("cannon").get_to(t.cannon);
  j.at("soldier").get_to(t.soldier);
}

void to_json(nlohmann::json &j, const GamePoints &g) {
  j = nlohmann::json{{"black", g.black}, {"red", g.red}};
}

void from_json(const nlohmann::json &j, GamePoints &g) {
  j.at("black").get_to(g.black);
  j.at("red").get_to(g.red);
}
} // namespace piece_points_spec

namespace json_io {

using namespace points_containers;

template <typename T>
void NlohmannExport(T object, string file_path) {
  nlohmann::json j;
  j = object;
  ofstream fout(file_path);
  fout << setw(4) << j << endl;
}

template <typename T>
T NlohmannImport(string file_path) {
  ifstream input(file_path);
  nlohmann::json j = nlohmann::json::parse(input);
  T imported_object = j.get<T>();
}

class NlohmannJsonIO : JsonIO {

  void Import(GamePoints &data, string file_path) override;
  void Export(GamePoints &data, string file_path) override;
  bool Validate(string data_file, string schema_file) override;

private:
  template <typename T>
  void ImportWithTemplate(T &object, string file_path) {
    ifstream input(file_path);
    nlohmann::json j = nlohmann::json::parse(input);
    object = j.get<T>();
  }

  template <typename T>
  void ExportWithTemplate(T &object, string file_path) {
    nlohmann::json j;
    j = object;
    ofstream fout(file_path);
    fout << setw(4) << j << endl;
  }
};

} // namespace json_io

#endif /* A64329FA_6D6E_4026_A7C1_FF253B8769B6 */
