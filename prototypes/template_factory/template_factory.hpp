#pragma once

#include <memory>
#include <my_template_class.hpp>
#include <string>
#include <unordered_map>

class TemplateFactory {
public:
  // Constructor
  TemplateFactory() = default;

  // Function to get or create template instance
  template <typename T>
  std::shared_ptr<MyTemplateClass<T>> getInstance(const std::string &key) {
    return std::make_shared<MyTemplateClass<T>>();
  }
};
