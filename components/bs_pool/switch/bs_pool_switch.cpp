#include "bs_pool_switch.h"

#include "esphome/core/log.h"

namespace esphome {
namespace bs_pool {

static const char *const TAG = "bs_pool.switch";

void BSPoolSwitch::dump_config() { ESP_LOGCONFIG(TAG, "BSPool:"); }

const std::vector<FunctionCode> BSPoolSwitch::codes_to_poll() {
  std::vector<FunctionCode> active_codes;
  if (this->user_is_outdoor_switch_ != nullptr ||
      this->user_cover_switch_off_switch_ != nullptr ||
      this->user_flow_switch_installed_switch_ != nullptr ||
      this->user_orp_displayed_switch_ != nullptr ||
      this->user_ph_alarm_switch_ != nullptr ||
      this->user_ph_corrector_alkaline_switch_ != nullptr ||
      this->user_ph_control_switch_ != nullptr ||
      this->user_cover_installed_switch_ != nullptr)
    active_codes.push_back(FunctionCode::USER);
  return active_codes;
}

void BSPoolSwitch::handle_message(DataPacket &message) {
  switch (message.function_code) {
    case FunctionCode::USER:
      this->last_user_byte_ = message.data_b2;
      this->has_user_readback_ = true;
      this->user_is_outdoor_switch_->publish_state(!(message.data_b2 & 0x01));
      this->user_cover_switch_off_switch_->publish_state(message.data_b2 & 0x02); // TODO check if this is correct
      this->user_flow_switch_installed_switch_->publish_state(message.data_b2 & 0x04);
      this->user_orp_displayed_switch_->publish_state(message.data_b2 & 0x08);
      this->user_ph_alarm_switch_->publish_state(!(message.data_b2 & 0x10));
      this->user_ph_corrector_alkaline_switch_->publish_state(message.data_b2 & 0x20);
      this->user_ph_control_switch_->publish_state(!(message.data_b2 & 0x40));
      this->user_cover_installed_switch_->publish_state(message.data_b2 & 0x80);
      break;
    default:
      break;
  }
}

void BSPoolSwitch::send_user_state() {
  if (!this->has_user_readback_) {
    ESP_LOGW(TAG, "Cannot send user settings - no readback yet");
    return;
  }
  uint8_t packed = this->last_user_byte_;
  if (this->user_is_outdoor_switch_ != nullptr) {
    if (this->user_is_outdoor_switch_->state) packed |= (1 << 0); else packed &= ~(1 << 0);
  }
  if (this->user_cover_switch_off_switch_ != nullptr) {
    if (this->user_cover_switch_off_switch_->state) packed |= (1 << 1); else packed &= ~(1 << 1);
  }
  if (this->user_flow_switch_installed_switch_ != nullptr) {
    if (this->user_flow_switch_installed_switch_->state) packed |= (1 << 2); else packed &= ~(1 << 2);
  }
  if (this->user_orp_displayed_switch_ != nullptr) {
    if (this->user_orp_displayed_switch_->state) packed |= (1 << 3); else packed &= ~(1 << 3);
  }
  if (this->user_ph_alarm_switch_ != nullptr) {
    if (!this->user_ph_alarm_switch_->state) packed |= (1 << 4); else packed &= ~(1 << 4);
  }
  if (this->user_ph_corrector_alkaline_switch_ != nullptr) {
    if (this->user_ph_corrector_alkaline_switch_->state) packed |= (1 << 5); else packed &= ~(1 << 5);
  }
  if (this->user_ph_control_switch_ != nullptr) {
    if (!this->user_ph_control_switch_->state) packed |= (1 << 6); else packed &= ~(1 << 6);
  }
  if (this->user_cover_installed_switch_ != nullptr) {
    if (this->user_cover_installed_switch_->state) packed |= (1 << 7); else packed &= ~(1 << 7);
  }
  this->parent_->enqueue_command(FunctionCode::USER, packed, '\4');
}

}  // namespace bs_pool
}  // namespace esphome
