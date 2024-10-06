#pragma once

#include <string>

using namespace std;

namespace piece_points {
class BPOPointsSKeys;
} // namespace piece_points

namespace jsonio {

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

  void Import(piece_points::BPOPointsSKeys &bpo_points, const string file_path) {
    static_cast<ConcreteJsonUtility *>(this)->ImplementImport(bpo_points, file_path);
  }

  void Export(piece_points::BPOPointsSKeys &bpo_points, const string file_path) {
    static_cast<ConcreteJsonUtility *>(this)->ImplementExport(bpo_points, file_path);
  }
};

} // namespace jsonio
