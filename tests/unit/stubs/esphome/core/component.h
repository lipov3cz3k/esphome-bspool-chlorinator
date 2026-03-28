#pragma once

#include <cstdint>

namespace esphome {

class Component {
 public:
  virtual void setup() {}
  virtual void loop() {}
  virtual void dump_config() {}
  virtual float get_setup_priority() const { return 0.0f; }
  virtual float get_loop_priority() const { return 0.0f; }
};

class PollingComponent : public Component {
 public:
  explicit PollingComponent(uint32_t update_interval = 0)
      : update_interval_(update_interval) {}
  virtual void update() {}
  uint32_t get_update_interval() const { return update_interval_; }

 protected:
  uint32_t update_interval_{0};
};

template<typename T>
class Parented {
 public:
  void set_parent(T *parent) { parent_ = parent; }
  T *get_parent() { return parent_; }

 protected:
  T *parent_{nullptr};
};

}

inline void delay(uint32_t) {}
