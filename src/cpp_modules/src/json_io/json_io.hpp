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
#include <piece_points_spec.hpp>
#include <string>
#include <typeinfo>
#include <unordered_map>

using namespace std;

namespace piece_points_spec {
void to_json(nlohmann::json &j, const TeamPointsNonTemp &t) {
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

void from_json(const nlohmann::json &j, TeamPointsNonTemp &t) {
  j.at("null").get_to(t.null);
  j.at("general").get_to(t.general);
  j.at("advisor").get_to(t.advisor);
  j.at("elephant").get_to(t.elephant);
  j.at("horse").get_to(t.horse);
  j.at("chariot").get_to(t.chariot);
  j.at("cannon").get_to(t.cannon);
  j.at("soldier").get_to(t.soldier);
}

void to_json(nlohmann::json &j, const GamePointsNonTemp &g) {
  j = nlohmann::json{{"black", g.black}, {"red", g.red}};
}

void from_json(const nlohmann::json &j, GamePointsNonTemp &g) {
  j.at("black").get_to(g.black);
  j.at("red").get_to(g.red);
}
} // namespace piece_points_spec

namespace json_io {

using namespace piece_points_spec;

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

class JsonIO {
public:
  virtual ~JsonIO() = default;

  virtual void Import(GamePointsNonTemp &data, string file_path) = 0;
  virtual void Export(GamePointsNonTemp &data, string file_path) = 0;
  virtual bool Validate(string data_file, string schema_file) = 0;
};

class NlohmannJsonIO : JsonIO {
public:
  void Import(GamePointsNonTemp &data, string file_path) override {
    PrivateImport(data, file_path);
  }
  void Export(GamePointsNonTemp &data, string file_path) override {
    PrivateExport(data, file_path);
  }
  bool Validate(string data_file, string schema_file) override {
    return PrivateValidate(data_file, schema_file);
  }

private:
  template <typename T>
  void PrivateImport(T &object, string file_path) {
    ifstream input(file_path);
    nlohmann::json j = nlohmann::json::parse(input);
    object = j.get<T>();
  }

  template <typename T>
  void PrivateExport(T &object, string file_path) {
    nlohmann::json j;
    j = object;
    ofstream fout(file_path);
    fout << setw(4) << j << endl;
  }

  bool PrivateValidate(string data_file, string schema_file) {

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
  //   unordered_map<string, any> Import(string file_path) override {
  //     unordered_map<string, any> imported_map;
  //     try {
  //       ifstream input(file_path);
  //       auto json_object = nlohmann::json::parse(input);
  //       imported_map = json_object;
  //     } catch (const exception &e) {
  //       cerr << "JSON import failed: " << e.what() << endl;
  //       exit(1);
  //     }
  //     return imported_map;
  // }

  //   template <typename Convertible>
  //   void Export(Convertible object, string file_path) {
  //     nlohmann::json output;
  //     try {
  //       output = object;
  //       ofstream fout(file_path);
  //       fout << setw(4) << output << endl;
  //     } catch (const exception &e) {
  //       cerr << "Unable to export object" << endl << e.what()
  //            << endl;
  //       exit(1);
  //     }
  //   }

  // private:
  //   void ExportJson(nlohmann::json j, string file_path) {
  //     ofstream fout(file_path);
  //     fout << setw(4) << j << endl;
  //   }
};

} // namespace json_io

#endif /* A64329FA_6D6E_4026_A7C1_FF253B8769B6 */
