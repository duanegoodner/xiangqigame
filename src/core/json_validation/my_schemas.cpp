
// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-20
// Last Modified: 2024-08-16

// Description:
// Dev script for validating format of piece points .json files 

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <nlohmann/json-schema.hpp>
#include <string>
#include <utility_functs.hpp>

using nlohmann::json;
using nlohmann::json_schema::json_validator;

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
      utility_functs::import_json(points_spec_ICGA_2004_path);
  auto points_spec_schema =
      utility_functs::import_json(points_spec_schema_path);

  auto raw_points_ICGA_2004 =
      utility_functs::import_json(raw_points_ICGA_2004_path);
  auto raw_points_schema = utility_functs::import_json(raw_points_schema_path);

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
