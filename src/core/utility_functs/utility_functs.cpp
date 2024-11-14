//! @file utility_functs.cpp
//! Implementation of the non-tempate functions defined in utility_functs.hpp.

#include <fstream>
#include <iostream>
#include <string>
#include <utility_functs.hpp>

const std::string utility_functs::get_data_file_abs_path(const std::string data_file) {
  const std::string abs_path = string(DATA_DIR) + "/" + data_file;
  return abs_path;
}


