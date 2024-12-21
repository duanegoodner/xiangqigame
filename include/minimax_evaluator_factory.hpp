#pragma once

#include <boost/functional/hash.hpp>
#include <move_evaluators.hpp>
#include <unordered_map.hpp>
#include <boardstate/zobrist.hpp>

namespace moveselection {
struct MinimaxTemplateSpecs {
  size_t key_size_bits;
  size_t num_conf_keys;

  bool operator==(const MinimaxTemplateSpecs &other) const {
    return key_size_bits == other.key_size_bits && num_conf_keys == other.num_conf_keys;
  }
};
} // namespace moveselection

namespace std {
template <>
struct hash<moveselection::MinimaxTemplateSpecs> {
  size_t operator()(const moveselection::MinimaxTemplateSpecs &specs) const {
    size_t seed = 0;
    boost::hash_combine(seed, specs.key_size_bits);
    boost::hash_combine(seed, specs.num_conf_keys);
    return seed;
  }
};
} // namespace std

// namespace moveselection {
// // class MinimaxEvaluatorFactory {
// //     std::unordered_map<>
// };
// } // namespace moveselection