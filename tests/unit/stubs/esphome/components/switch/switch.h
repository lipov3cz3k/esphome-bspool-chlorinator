#pragma once

#include "esphome/core/entity_base.h"

namespace esphome {
namespace switch_ {

class Switch : public EntityBase {
 public:
  virtual void publish_state(bool value) { state = value; }
  virtual void write_state(bool) {}

  bool state{false};
};

}
}

#define SUB_SWITCH(name)                                               \
 protected:                                                            \
  ::esphome::switch_::Switch *name##_switch_{nullptr};                \
                                                                        \
 public:                                                               \
  void set_##name##_switch(::esphome::switch_::Switch *s) {           \
    name##_switch_ = s;                                                \
  }
