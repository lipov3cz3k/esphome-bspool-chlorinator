#pragma once

#include "esphome/core/entity_base.h"

namespace esphome {
namespace sensor {

class Sensor : public EntityBase {
 public:
  virtual void publish_state(float value) {
    last_value = value;
    has_value_ = true;
  }

  float last_value{0.0f};
  bool has_value_{false};
};

}
}

#define SUB_SENSOR(name)                                                 \
 protected:                                                              \
  ::esphome::sensor::Sensor *name##_sensor_{nullptr};                   \
                                                                          \
 public:                                                                 \
  void set_##name##_sensor(::esphome::sensor::Sensor *s) {              \
    name##_sensor_ = s;                                                  \
  }
