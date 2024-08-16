// Filename: json_internal.tpp
// Author: Duane Goodner
// Created: 2022-12-22
// Last Modified: 2024-08-16

// Description:
// Implementations of methods for JsonInternal template class.

#ifndef F976D623_CC94_4D09_9AE1_B4340502DC80
#define F976D623_CC94_4D09_9AE1_B4340502DC80

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using nloh_json = nlohmann::json;

namespace json_internal {

template <>
inline nloh_json import_json<nloh_json>(string file_path) {
  ifstream input(file_path);
  return nloh_json::parse(input);
}

template <>
inline void export_json<nloh_json>(const nloh_json &j, string file_path) {
  ofstream fout(file_path);
  fout << setw(4) << j << endl;
}

template <typename IOType, typename SchemaType>
inline IOType JsonInternal<IOType, SchemaType>::Import(string file_path) {
  return import_json<IOType>(file_path);
}

template <typename IOType, typename SchemaType>
inline void JsonInternal<IOType, SchemaType>::Export(IOType &j, string file_path) {
  export_json<IOType>(j, file_path);
}

template <typename IOType, typename SchemaType>
inline bool JsonInternal<IOType, SchemaType>::Validate(
    string json_file,
    string schema_file
) {
  return validate_json_schema<SchemaType>(json_file, schema_file);
}

} // namespace json_internal

#endif /* F976D623_CC94_4D09_9AE1_B4340502DC80 */
