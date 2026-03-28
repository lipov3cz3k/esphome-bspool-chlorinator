#pragma once

#include <cstdint>
#include <string>

#include <gtest/gtest.h>

#include "../../components/bs_pool/bs_pool.h"
#include "../../components/bs_pool/switch/bs_pool_switch.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/select/select.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace bs_pool {
namespace testing {

extern uint32_t millis_value;

class MockSensor : public sensor::Sensor {
 public:
  float value{0.0f};
  bool published{false};
  void publish_state(float v) override {
    value = v;
    published = true;
  }
};

class MockBinarySensor : public binary_sensor::BinarySensor {
 public:
  bool value{false};
  bool published{false};
  void publish_state(bool v) override {
    value = v;
    published = true;
  }
};

class MockSwitch : public switch_::Switch {
 public:
  bool value{false};
  bool published{false};
  void publish_state(bool v) override {
    value = v;
    published = true;
  }
};

class MockSelect : public select::Select {
 public:
  std::string value;
  bool published{false};
  void publish_state(const std::string &v) override {
    value = v;
    published = true;
  }
};

class MockTextSensor : public text_sensor::TextSensor {
 public:
  std::string value;
  bool published{false};
  void publish_state(const std::string &v) override {
    value = v;
    published = true;
  }
};

inline DataPacket make_packet(FunctionCode code, uint8_t b2, uint8_t b3) {
  DataPacket p;
  p.function_code = static_cast<uint8_t>(code);
  p.data_b2 = b2;
  p.data_b3 = b3;
  return p;
}

struct SwitchSet {
  MockSwitch user_is_outdoor;
  MockSwitch user_cover_switch_off;
  MockSwitch user_flow_switch_installed;
  MockSwitch user_orp_displayed;
  MockSwitch user_ph_alarm;
  MockSwitch user_ph_corrector_alkaline;
  MockSwitch user_ph_control;
  MockSwitch user_cover_installed;
};

inline BSPoolSwitch *make_switch_with_all_mocks(SwitchSet &mocks) {
  auto *sw = new BSPoolSwitch();
  sw->set_user_is_outdoor_switch(&mocks.user_is_outdoor);
  sw->set_user_cover_switch_off_switch(&mocks.user_cover_switch_off);
  sw->set_user_flow_switch_installed_switch(&mocks.user_flow_switch_installed);
  sw->set_user_orp_displayed_switch(&mocks.user_orp_displayed);
  sw->set_user_ph_alarm_switch(&mocks.user_ph_alarm);
  sw->set_user_ph_corrector_alkaline_switch(&mocks.user_ph_corrector_alkaline);
  sw->set_user_ph_control_switch(&mocks.user_ph_control);
  sw->set_user_cover_installed_switch(&mocks.user_cover_installed);
  return sw;
}

}
}
}
