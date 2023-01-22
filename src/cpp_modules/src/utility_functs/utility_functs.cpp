#include <fstream>
#include <string>
#include <utility_functs.hpp>
#include <nlohmann/json.hpp>


using namespace std;
using json = nlohmann::json;

json utility_functs::import_json(string file_path) {
  ifstream input(file_path);
  return json::parse(input);
}

void utility_functs::export_json(const json &j, string filename) {
  ofstream fout(filename);
  fout << setw(4) << j << endl;
}
