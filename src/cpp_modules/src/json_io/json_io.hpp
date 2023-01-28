#ifndef A64329FA_6D6E_4026_A7C1_FF253B8769B6
#define A64329FA_6D6E_4026_A7C1_FF253B8769B6

#include <any>
#include <common.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <piece_points_spec.hpp>
#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using namespace std;
using namespace piece_points_spec;

namespace json_io {



class JsonIO {
public:
  virtual ~JsonIO() = default;
  virtual unordered_map<string, any> Import(string file_path) = 0;
  virtual void Export(any data) = 0;
  virtual bool IsExportable(any object) = 0;
  virtual bool Validate(string data_file, string schema_file) = 0;

};

class NlohmannExportMethods {
  void ExportGamePoints(GamePointsNonTemp game_points) {}

};

// class NlohmannJsonIO : JsonIO {
//   public:
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
//   }

//   void Export(unordered_map<string, any> data) override {
//     nlohmann::json output;
//     for (auto key : data) {
//       output[key] = data.second;
//     }
//   }

//   private:
//   typedef void
//   unordered_map<size_t, >
// };

} // namespace json_io

#endif /* A64329FA_6D6E_4026_A7C1_FF253B8769B6 */
