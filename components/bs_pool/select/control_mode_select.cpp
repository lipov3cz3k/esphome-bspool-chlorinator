#include "control_mode_select.h"

namespace esphome {
namespace bs_pool {

void ControlModeSelect::control(const std::string &value) {
  auto index = this->index_of(value);
  if (index.has_value()) {
    this->publish_state(value);
    this->parent_->send_command(FunctionCode::CONTROL_MODE,
                                static_cast<uint8_t>(index.value()));
  }
}

}  // namespace bs_pool
}  // namespace esphome
