#pragma once

#include <string>

#include "esphome/core/entity_base.h"

namespace esphome {
namespace text_sensor {

class TextSensor : public EntityBase {
 public:
  virtual void publish_state(const std::string &value) {
    last_value = value;
    has_value_ = true;
  }

  std::string last_value;
  bool has_value_{false};
};

}
}

#define SUB_TEXT_SENSOR(name)                                                  \
 protected:                                                                    \
  ::esphome::text_sensor::TextSensor *name##_text_sensor_{nullptr};           \
                                                                                \
 public:                                                                       \
  void set_##name##_text_sensor(::esphome::text_sensor::TextSensor *s) {      \
    name##_text_sensor_ = s;                                                   \
  }
