#ifndef F976D623_CC94_4D09_9AE1_B4340502DC80
#define F976D623_CC94_4D09_9AE1_B4340502DC80

#include <fstream>
#include <nlohmann/json.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/schema.h>
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
inline rapidjson::Document import_json<rapidjson::Document>(string file_path) {
  ifstream is(file_path);
  rapidjson::IStreamWrapper is_wrapper(is);
  rapidjson::Document d;
  d.ParseStream(is_wrapper);
  return d;
}

template <>
inline void export_json<nloh_json>(const nloh_json &j, string file_path) {
  ofstream fout(file_path);
  fout << setw(4) << j << endl;
}

template<>
inline bool validate_json_schema<rapidjson::Document>(
  rapidjson::Document& data_json,
  string schema_file
) {
  auto schema_json = import_json<rapidjson::Document>(schema_file);
  rapidjson::SchemaDocument schema(schema_json);
  rapidjson::SchemaValidator validator(schema);
  return data_json.Accept(validator);
}

template <>
inline bool validate_json_schema<rapidjson::Document>(
    string json_file,
    string schema_file
) {
  auto data_json = import_json<rapidjson::Document>(json_file);

  auto schema_json = import_json<rapidjson::Document>(schema_file);
  auto data = import_json<rapidjson::Document>(json_file);
  rapidjson::SchemaDocument schema(schema_json);
  rapidjson::SchemaValidator validator(schema);

  return data_json.Accept(validator);
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
