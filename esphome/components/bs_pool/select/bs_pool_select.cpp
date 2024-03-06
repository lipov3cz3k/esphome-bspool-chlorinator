#include "bs_pool_select.h"

#include <map>
#include <string>

#include "esphome/core/log.h"

namespace esphome {
namespace bs_pool {

static const char *const TAG = "bs_pool.select";

void BSPoolSelect::dump_config() { ESP_LOGCONFIG(TAG, "BSPool:"); }

const std::vector<FunctionCode> BSPoolSelect::codes_to_poll() {
  std::vector<FunctionCode> active_codes;
  if (this->control_mode_select_ != nullptr)
    active_codes.push_back(FunctionCode::CONTROL_MODE);
  return active_codes;
}

void BSPoolSelect::handle_message(DataPacket &message) {
  switch (message.function_code) {
    case FunctionCode::CONTROL_MODE:
      if (this->control_mode_select_ != nullptr) {
        auto selected = this->control_mode_select_->at(
            static_cast<size_t>(message.data_b2));
        if (selected.has_value())
          this->control_mode_select_->publish_state(selected.value());
        else
          ESP_LOGW(TAG, "Received unknown control mode: %d", message.data_b2);
      }
      break;
  }
}

void BSPoolSelect::send_command(FunctionCode code, char b2, char b3) {
  this->parent_->write_array({code, b2, b3});
  this->parent_->flush();
}

}  // namespace bs_pool
}  // namespace esphome
