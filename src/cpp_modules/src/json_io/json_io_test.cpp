#include <common.hpp>
#include <gtest/gtest.h>
#include <json_interface.hpp>
#include <json_io.hpp>

using namespace std;
class NlohmannJsonIOTest : public ::testing::Test {
protected:
  string kICGARawPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_raw.json";

  string kRawSchemaPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                          "raw_points_schema.json";

  string kICGABPOPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "bpo_schema.json";

  string kBPOSchemaPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                          "bpo_schema.json";
};

TEST_F(NlohmannJsonIOTest, Init) { json_io::NlohmannJsonIO json; }

TEST_F(NlohmannJsonIOTest, CastToInterfaceRef) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;
}

TEST_F(NlohmannJsonIOTest, ImportRawPoints) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;
  GamePointsSMap_t game_points_map{};
  json_interface.Import(game_points_map, kICGARawPath);
}

TEST_F(NlohmannJsonIOTest, ValidateDataThatSatisfiesSchema) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;
  auto validation_result =
      json_interface.Validate(kICGARawPath, kRawSchemaPath);
  EXPECT_TRUE(validation_result);
}

TEST_F(NlohmannJsonIOTest, RejectDataToSchemaMismatch) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;
  auto validation_result =
      json_interface.Validate(kICGARawPath, kBPOSchemaPath);
  EXPECT_FALSE(validation_result);
}
