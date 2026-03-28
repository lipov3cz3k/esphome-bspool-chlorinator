#pragma once

#include <optional>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "esphome/core/entity_base.h"

namespace esphome {
namespace select {

class Select : public EntityBase {
 public:
  virtual void publish_state(const std::string &value) {
    last_value = value;
    has_value_ = true;
  }

  virtual void control(const std::string &) {}

  std::optional<std::string> at(size_t idx) const {
    if (idx < options_.size()) {
      return options_[idx];
    }
    return {};
  }

  std::optional<size_t> index_of(const std::string &value) const {
    for (size_t i = 0; i < options_.size(); i++) {
      if (options_[i] == value) {
        return i;
      }
    }
    return {};
  }

  void traits() {}
  void set_options(std::vector<std::string> opts) { options_ = std::move(opts); }

  std::string last_value;
  bool has_value_{false};

 protected:
  std::vector<std::string> options_;
};

}
}

#define SUB_SELECT(name)                                                \
 protected:                                                             \
  ::esphome::select::Select *name##_select_{nullptr};                  \
                                                                         \
 public:                                                                \
  void set_##name##_select(::esphome::select::Select *s) {             \
    name##_select_ = s;                                                 \
  }
