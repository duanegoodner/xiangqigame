
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <nlohmann/json-schema.hpp>
#include <string>
#include <utility_functs.hpp>

using nlohmann::json;
using nlohmann::json_schema::json_validator;

static std::string points_spec_ICGA_2004_path =
    "/home/duane/workspace/project/src/cpp_modules/data/"
    "ICGA_2004_bpo.json";

static std::string raw_points_ICGA_2004_path =
    "/home/duane/workspace/project/src/cpp_modules/data/"
    "ICGA_2004_raw.json";

static std::string points_spec_schema_path =
    "/home/duane/workspace/project/src/cpp_modules/data/"
    "bpo_schema.json";

static std::string raw_points_schema_path =
    "/home/duane/workspace/project/src/cpp_modules/data/"
    "raw_points_schema.json";

int main() {

  auto points_spec_ICGA_2004 =
      utility_functs::import_json(points_spec_ICGA_2004_path);
  auto points_spec_schema =
      utility_functs::import_json(points_spec_schema_path);

  auto raw_points_ICGA_2004 =
      utility_functs::import_json(raw_points_ICGA_2004_path);
  auto raw_points_schema = utility_functs::import_json(raw_points_schema_path);

  json_validator spec_validator;

  try {
    spec_validator.set_root_schema(points_spec_schema);
  } catch (const std::exception &e) {
    std::cerr << "Points Spec schema validation failed: " << e.what()
              << std::endl;
    return EXIT_FAILURE;
  }

  try {
    
    auto spec_validation_result = spec_validator.validate(raw_points_ICGA_2004);
    std::cout << "ICGA 2004 points spec validation succeeded" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "ICGA 2004 points spec validation failed: " << e.what()
              << std::endl;
  }

  json_validator raw_points_validator;

  return EXIT_SUCCESS;
}
