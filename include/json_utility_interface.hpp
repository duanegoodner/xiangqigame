//! @file json_utility_interface.hpp
//! Definition of JsonUtility CRTP interface.

#pragma once

#include <string>

using namespace std;

namespace piecepoints {
class BPOPointsSKeys;
} // namespace piecepoints

//! CRTP interface with methods of importing / exporting objects from / to json files
//! (concrete example = jsonio::NlohmannJsonUtility).
template <typename ConcreteJsonUtility>
class JsonUtility {
public:
  template <typename T>
  void Import(T &object, const string file_path) {
    static_cast<ConcreteJsonUtility *>(this)->ImplementImport(object, file_path);
  }

  template <typename T>
  void Export(T &object, const string file_path) {
    static_cast<ConcreteJsonUtility *>(this)->ImplementExport(object, file_path);
  }

  void Import(piecepoints::BPOPointsSKeys &bpo_points, const string file_path) {
    static_cast<ConcreteJsonUtility *>(this)->ImplementImport(bpo_points, file_path);
  }

  void Export(piecepoints::BPOPointsSKeys &bpo_points, const string file_path) {
    static_cast<ConcreteJsonUtility *>(this)->ImplementExport(bpo_points, file_path);
  }
};
