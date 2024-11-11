//! @file my_schemas.cpp 
//! json validation gist, unused by rest of project, but keeping here for reference. 

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <nlohmann/json-schema.hpp>
#include <string>
#include <json_utility_interface.hpp>
#include <json_utility_nlohmann.hpp>

using nlohmann::json;
using nlohmann::json_schema::json_validator;
using namespace jsonio;

static const std::string points_spec_ICGA_2004_path = utility_functs::get_data_file_abs_path(
  "ICGA_2004_bpo.json");
static const std::string raw_points_ICGA_2004_path = utility_functs::get_data_file_abs_path(
  "ICGA_2004_raw.json");
static const std::string points_spec_schema_path = utility_functs::get_data_file_abs_path(
  "bpo_schema.json");
static const std::string raw_points_schema_path = utility_functs::get_data_file_abs_path(
  "raw_points_schema.json");

int main() {

  auto points_spec_ICGA_2004 =
      import_json(points_spec_ICGA_2004_path);
  auto points_spec_schema =
      import_json(points_spec_schema_path);

  auto raw_points_ICGA_2004 =
      import_json(raw_points_ICGA_2004_path);
  auto raw_points_schema = import_json(raw_points_schema_path);

  json_validator spec_validator;

  try {
    spec_validator.set_root_schema(raw_points_schema);
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
