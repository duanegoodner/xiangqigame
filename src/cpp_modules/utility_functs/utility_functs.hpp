#ifndef _UTILITY_FUNCTS_
#define _UTILITY_FUNCTS_

#include <array>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

namespace utility_functs {

using json = nlohmann::json;
using namespace std;

template <typename T>
T random(T range_from, T range_to) {
  random_device rand_dev;
  mt19937 generator(rand_dev());
  uniform_int_distribution<T> distr(range_from, range_to);
  return distr(generator);
}

inline bool ends_with(string const &value, string const &ending) {
  if (ending.size() > value.size())
    return false;
  return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

template <typename FromKey, typename ToKey, typename Value>
unordered_map<ToKey, Value> replace_keys(
    unordered_map<FromKey, Value> orig_map, std
    ::unordered_map<FromKey, ToKey> key_substitutions
) {
  unordered_map<ToKey, Value> new_map;
  for (auto entry : key_substitutions) {
    new_map[entry.second] = orig_map[entry.first];
  }
  return new_map;
}

// replaces KEYS in original map with VALUES of matching KEYS in key_substitutions
template <typename FromKey, typename ToKey, typename Value>
unordered_map<ToKey, Value> replace_keys_forward(
    unordered_map<FromKey, Value> orig_map, unordered_map<FromKey, ToKey> key_substitutions
) {
  unordered_map<ToKey, Value> new_map;
  for (auto entry : key_substitutions) {
    new_map[entry.second] = orig_map[entry.first];
  }
  return new_map;
}

// replaces KEYS in original map with KEYS of matching VALUES in key_substitutions
template <typename FromKey, typename ToKey, typename Value>
unordered_map<ToKey, Value> replace_keys_reverse(
  unordered_map<FromKey, Value> orig_map, unordered_map<ToKey, FromKey> key_substitutions
) {
  unordered_map<ToKey, Value> new_map;
  for (auto entry : key_substitutions) {
    new_map[entry.first] = orig_map[entry.second];
  }
  return new_map;
}

template <typename two_d_array_t>
two_d_array_t vertical_flip_array(
  two_d_array_t orig_array
) {
  auto flipped_array = orig_array;
  reverse(flipped_array.begin(), flipped_array.end());
  return flipped_array;
}

template <typename two_d_array_t>
two_d_array_t two_array_sum(two_d_array_t a, two_d_array_t b) {
  two_d_array_t result{};
  for (auto row = 0; row < a.size(); row++) {
    for (auto col = 0; col < a[0].size(); col++) {
      result[row][col] = a[row][col] + b[row][col];
    }
  }
  return result;
}

template <typename two_d_array_t, typename array_element_t>
two_d_array_t array_plus_const(two_d_array_t array, array_element_t offset) {
  two_d_array_t result{};
  for (auto row = 0; row < array.size(); row++) {
    for (auto col = 0; col < array[0].size(); col++) {
      result[row][col] = array[row][col] + offset;
    }
  }
  return result;
}

json import_json(string file_path);
void export_json(const json &j, string filename);

} // namespace utility_functs

#endif // _UTILITY_FUNCTS_
