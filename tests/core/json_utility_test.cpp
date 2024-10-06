#include <filesystem>
#include <gtest/gtest.h>
#include <json_utility_interface.hpp>
#include <json_utility_nlohmann.hpp>
#include <piece_points_spec.hpp>
#include <random>
#include <sstream>
#include <unordered_map>

namespace fs = std::filesystem;

class JsonUtilityTest : public ::testing::Test {
protected:
  fs::path temp_dir;
  jsonio::NlohmannJsonUtility nlohmann_json_utility{};

  // Use lambda function because hpp2plantuml can't parse {{
  const unordered_map<string, int> test_map = [] {
    unordered_map<string, int> temp;
    temp.insert(make_pair("one", 1));
    temp.insert(make_pair("two", 2));
    temp.insert(make_pair("black", 3));
    return temp;
  }();

  void SetUp() override {
    // Create a unique directory name using a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    std::stringstream ss;
    ss << "temp_test_dir_" << dis(gen);
    temp_dir = fs::temp_directory_path() / ss.str();
    fs::create_directories(temp_dir);
  }

  void TearDown() override { fs::remove_all(temp_dir); }
};

TEST_F(JsonUtilityTest, ExportSimpleMap) {
  std::string test_map_path = (temp_dir / "testa_map.json").string();
  EXPECT_FALSE(filesystem::exists(test_map_path));
  nlohmann_json_utility.Export(test_map, test_map_path);
  EXPECT_TRUE(filesystem::exists(test_map_path));
}

TEST_F(JsonUtilityTest, ExportImportSimpleMap) {
  std::string test_map_path = (temp_dir / "testa_map.json").string();
  
  EXPECT_FALSE(filesystem::exists(test_map_path));
  nlohmann_json_utility.Export(test_map, test_map_path);
  EXPECT_TRUE(filesystem::exists(test_map_path));

  unordered_map<string, int> re_imported_test_map{};
  nlohmann_json_utility.Import(re_imported_test_map, test_map_path);
  EXPECT_EQ(re_imported_test_map["one"], 1);
}

TEST_F(JsonUtilityTest, ImportBPOPoints) {
  piece_points::BPOPointsSKeys bpo_points{};
  nlohmann_json_utility.Import(bpo_points, piece_points::kICGABPOPath);
  EXPECT_EQ(bpo_points.black_base_["horse"], 270);
  EXPECT_EQ(bpo_points.red_base_offsets_["horse"], 0);
}

TEST_F(JsonUtilityTest, ImportExportBPOPoints) {
  piece_points::BPOPointsSKeys bpo_points{};
  nlohmann_json_utility.Import(bpo_points, piece_points::kICGABPOPath);

  std::string file_path = (temp_dir / "bpo_output.json").string();
  nlohmann_json_utility.Export(bpo_points, file_path);
}

TEST_F(JsonUtilityTest, ImportExportImportBPOPoints) {
  piece_points::BPOPointsSKeys bpo_points{};
  nlohmann_json_utility.Import(bpo_points, piece_points::kICGABPOPath);

  std::string file_path = (temp_dir / "bpo_output.json").string();
  nlohmann_json_utility.Export(bpo_points, file_path);

  piece_points::BPOPointsSKeys bpo_points_second_import{};
  nlohmann_json_utility.Import(bpo_points_second_import, file_path);
}

TEST_F(JsonUtilityTest, ImportGamePointsSMap) {
  GamePointsSMap_t s_map;
  nlohmann_json_utility.Import(s_map, piece_points::kICGARawPath);
  EXPECT_EQ(s_map["black"]["horse"][0][0], 270);
  EXPECT_EQ(s_map["red"]["horse"][0][0], 274);
}