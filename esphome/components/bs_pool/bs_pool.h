#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace bs_pool {

enum FunctionCode : char {
  // Sensors
  CELL_CURRENT_MEASUREMENT = 'c',
  CELL_INTENSITY_MEASUREMENT = 'C',
  CELL_VOLTAGE_MEASUREMENT = 'V',
  PH_MEASUREMENT = 'p',
  SALT_CONCENTRATION_MEASUREMENT = 'N',
  TIME = 'H',
  SOFTWARE_VERSION = 'y',
};

union DataPacket {
  uint8_t raw[3];
  struct {
    uint8_t function_code;
    uint8_t data_b2;
    uint8_t data_b3;
  };
} __attribute__((packed));

class BSPoolListener {
 public:
  virtual void handle_message(DataPacket &message) = 0;
  virtual const std::vector<FunctionCode> codes_to_poll() = 0;
};

class BSPool : public uart::UARTDevice, public PollingComponent {
 public:
  void update() override;
  void loop() override;

  void register_listener(BSPoolListener *listener) {
    this->listeners_.push_back(listener);
  }

 protected:
  DataPacket buffer_;

  std::vector<BSPoolListener *> listeners_{};
};

}  // namespace bs_pool
}  // namespace esphome
