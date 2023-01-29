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

class NlohmannJsonIO : public JsonIO {

  public:
  GamePointsSMap_t Import(string file_path) override;
  void Export(GamePointsSMap_t &data, string file_path) override;
  bool Validate(string data_file, string schema_file) override;

private:
  template <typename T>
  T ImportWithTemplate(string file_path) {
    ifstream input(file_path);
    nlohmann::json j = nlohmann::json::parse(input);
    auto object = j.get<T>();
    return object;
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
