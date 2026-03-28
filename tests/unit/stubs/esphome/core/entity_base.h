#pragma once

#include <string>

namespace esphome {

class EntityBase {
 public:
  void set_name(const char *name) { name_ = name == nullptr ? "" : name; }
  void set_object_id(const char *id) { object_id_ = id == nullptr ? "" : id; }
  const std::string &get_name() const { return name_; }

 protected:
  std::string name_;
  std::string object_id_;
};

}
