#pragma once

#include "esphome/core/entity_base.h"

namespace esphome {
namespace binary_sensor {

class BinarySensor : public EntityBase {
 public:
  virtual void publish_state(bool value) {
    last_value = value;
    has_value_ = true;
  }

  bool last_value{false};
  bool has_value_{false};
};

}
}

#define SUB_BINARY_SENSOR(name)                                                      \
 protected:                                                                          \
  ::esphome::binary_sensor::BinarySensor *name##_binary_sensor_{nullptr};           \
                                                                                      \
 public:                                                                             \
  void set_##name##_binary_sensor(::esphome::binary_sensor::BinarySensor *s) {      \
    name##_binary_sensor_ = s;                                                       \
  }
