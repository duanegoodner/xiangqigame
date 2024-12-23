#include <jsonvalidation/json_validation.hpp>
#include <fstream>
#include <iostream>
#include <string>

nlohmann::json import_json(std::string file_path) {
  std::ifstream input(file_path);
  return nlohmann::json::parse(input);
}

bool json_matches_schema(
    const nlohmann::json &imported_data,
    const nlohmann::json &schema
) {
  nlohmann::json_schema::json_validator validator;

  try {
    validator.set_root_schema(schema);
  } catch (const std::exception &e) {
    std::cerr << "Schema validation failed: " << e.what() << std::endl;
    exit(1);
  }

  try {
    auto data_validation_result = validator.validate(imported_data);
    return true;
  } catch (const std::exception &e) {
    return false;
  }
}
