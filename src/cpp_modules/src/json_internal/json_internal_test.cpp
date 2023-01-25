#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <gtest/gtest.h>
#include <iostream>
#include <json_internal.hpp>
#include <limits>
#include <nlohmann/json.hpp>
#include <string>
#include <utility_functs.hpp>

using namespace std;
using namespace json_internal;
using nloh_json = nlohmann::json;

class JsonInternalTest : public ::testing::Test {
protected:
  string kICGABPOPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_bpo.json";

  string kICGARawPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_raw.json";

  string kRawSchemaPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                          "raw_points_schema.json";

  string kBPOSchemaPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                          "bpo_schema.json";
};

TEST_F(JsonInternalTest, NlohmannImport) {
  auto nloh_bpo_data = import_json<nloh_json>(kICGABPOPath);
  cout << "done" << endl;
}

TEST_F(JsonInternalTest, RapidJsonImport) {
  auto rapidjson_bpo_data = import_json<rapidjson::Document>(kICGABPOPath);
}

TEST_F(JsonInternalTest, NlohmannExport) {
  auto nloh_bpo_data = import_json<nloh_json>(kICGABPOPath);
  auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
  auto temp_path = testing::TempDir();
  string prefix{"GTEST-"};

  temp_path.append(prefix);
  size_t random_int =
      utility_functs::random((size_t)0, (size_t)numeric_limits<size_t>::max);
  temp_path.append(to_string(random_int));
  temp_path.append(".json");
  cout << temp_path << endl;
  export_json<nloh_json>(nloh_bpo_data, temp_path);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}