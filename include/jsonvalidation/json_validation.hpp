#pragma once

#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>

nlohmann::json import_json(std::string file_path);
bool json_matches_schema(const nlohmann::json &json_object, const nlohmann::json &schema);




