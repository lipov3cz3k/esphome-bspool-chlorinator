#include "bs_pool_text_sensor.h"

#include <string>

#include "esphome/core/log.h"

namespace esphome {
namespace bs_pool {

static const char *const TAG = "bs_pool.text_sensor";

void BSPoolTextSensor::dump_config() { ESP_LOGCONFIG(TAG, "BSPool:"); }

const std::vector<FunctionCode> BSPoolTextSensor::codes_to_poll() {
  std::vector<FunctionCode> active_codes;
  if (this->time_text_sensor_ != nullptr)
    active_codes.push_back(FunctionCode::TIME);
  if (this->version_text_sensor_ != nullptr)
    active_codes.push_back(FunctionCode::SOFTWARE_VERSION);
  return active_codes;
}

void BSPoolTextSensor::handle_message(DataPacket &message) {
  switch (message.function_code) {
    case FunctionCode::TIME:
      if (this->time_text_sensor_ != nullptr){
        std::string device_time = std::to_string(message.data_b3) + ":" +
                                  (message.data_b2 < 10 ? "0" : "") + std::to_string(message.data_b2);
        this->time_text_sensor_->publish_state(device_time);
      }
      break;
    case FunctionCode::SOFTWARE_VERSION:
      if (this->version_text_sensor_ != nullptr) {
        std::string version = std::to_string(message.data_b2) + "." + std::to_string(message.data_b3);
        this->version_text_sensor_->publish_state(version);
      }
      break;
  }
}

}  // namespace bs_pool
}  // namespace esphome
