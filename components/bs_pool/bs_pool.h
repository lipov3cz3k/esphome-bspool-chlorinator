#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace bs_pool {

enum FunctionCode : char {
  // Switches
  USER = 'r',
  // Select
  CONTROL_MODE = 'm',
  // Binary sensors
  ALARMS = 'A',
  WARNINGS = 'w',
  STATUS = 's',
  // Sensors
  CELL_CURRENT_MEASUREMENT = 'c',
  CELL_INTENSITY_MEASUREMENT = 'C',
  CELL_VOLTAGE_MEASUREMENT = 'V',
  PH_MEASUREMENT = 'p',
  SALT_CONCENTRATION_MEASUREMENT = 'N',
  TIME = 'H',
  SOFTWARE_VERSION = 'y',
  RADOX_MEASUREMENT = 'o',
  TEMPERATURE_MEASUREMENT = 'W',
};

union DataPacket {
  uint8_t raw[3];
  struct {
    uint8_t function_code;
    uint8_t data_b2;
    uint8_t data_b3;
  };
} __attribute__((packed));

enum CommandType : uint8_t { POLL, WRITE };

struct CommandEntry {
  CommandType type;
  FunctionCode code;
  uint8_t b2;
  uint8_t b3;
};

enum MachineState : uint8_t { IDLE, SENDING, WAITING };

class BSPoolListener {
 public:
  virtual void handle_message(DataPacket &message) = 0;
  virtual const std::vector<FunctionCode> codes_to_poll() = 0;
};

class BSPool : public uart::UARTDevice, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void loop() override;
  void enqueue_command(FunctionCode code, uint8_t b2, uint8_t b3);

  void register_listener(BSPoolListener *listener) {
    this->listeners_.push_back(listener);
  }

 protected:
  static constexpr uint8_t MAX_QUEUE_SIZE = 16;
  static constexpr uint32_t RESPONSE_TIMEOUT_MS = 200;
  static constexpr uint32_t INTER_COMMAND_GAP_MS = 10;

  CommandEntry queue_[MAX_QUEUE_SIZE];
  uint8_t queue_head_{0};
  uint8_t queue_size_{0};
  MachineState state_{IDLE};
  uint32_t last_command_time_{0};
  CommandEntry current_command_{};

  DataPacket buffer_;

  std::vector<BSPoolListener *> listeners_{};

 private:
  bool enqueue_back_(CommandEntry entry);
  bool enqueue_front_(CommandEntry entry);
  CommandEntry dequeue_();
};

}  // namespace bs_pool
}  // namespace esphome
