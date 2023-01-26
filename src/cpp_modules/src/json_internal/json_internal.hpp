#ifndef B849AC13_4E45_4022_A67D_F98FD91B83A8
#define B849AC13_4E45_4022_A67D_F98FD91B83A8

#include <nlohmann/json.hpp>
#include <string>

using namespace std;

namespace json_internal {

template <typename JsonType>
JsonType import_json(string file_path);

template <typename JsonType>
void export_json(const JsonType& j, string file_path);

template <typename JsonType>
bool validate_json_schema(JsonType & data_json, string schema_file);

template <typename JsonType>
bool validate_json_schema(string json_file, string schema_file);

template <typename IOType, typename SchemaType>
class JsonInternal {
  IOType Import(string file_path);
  void Export(IOType& j, string file_path);
  bool Validate(string json_file, string schema);
};

} // namespace json_internal

#include <json_internal.tpp>


#endif /* B849AC13_4E45_4022_A67D_F98FD91B83A8 */
