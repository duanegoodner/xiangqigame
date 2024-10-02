#include <common.hpp>
#include <gtest/gtest.h>
#include <json_interface.hpp>
#include <json_io.hpp>
#include <utility_functs.hpp>

using namespace std;
class NlohmannJsonIOTest : public ::testing::Test {
protected:
  const string kICGARawPath =
      utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");

  const string kRawSchemaPath =
      utility_functs::get_data_file_abs_path("raw_points_schema.json");
  const string kICGABPOPath =
      utility_functs::get_data_file_abs_path("bpo_schema.json");
  const string kBPOSchemaPath =
      utility_functs::get_data_file_abs_path("bpo_schema.json");
};

TEST_F(NlohmannJsonIOTest, Init) { json_io::NlohmannJsonIO json; }

TEST_F(NlohmannJsonIOTest, CastToInterfaceRef) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonInterface &json_interface = json;
}

TEST_F(NlohmannJsonIOTest, ImportRawPoints) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonInterface &json_interface = json;
  GamePointsSMap_t game_points_map{};
  json_interface.Import(game_points_map, kICGARawPath);
}

TEST_F(NlohmannJsonIOTest, ValidateDataThatSatisfiesSchema) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonInterface &json_interface = json;
  auto validation_result =
      json_interface.Validate(kICGARawPath, kRawSchemaPath);
  EXPECT_TRUE(validation_result);
}

TEST_F(NlohmannJsonIOTest, RejectDataToSchemaMismatch) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonInterface &json_interface = json;
  auto validation_result =
      json_interface.Validate(kICGARawPath, kBPOSchemaPath);
  EXPECT_FALSE(validation_result);
}
