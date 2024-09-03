#include "bs_pool_switch.h"

#include <bitset>
#include <map>
#include <string>

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
  UserSettings user_settings = {
      .is_outdoor = this->user_is_outdoor_switch_->state,
      .cover_switch_off = this->user_cover_switch_off_switch_->state,
      .flow_switch_installed = this->user_flow_switch_installed_switch_->state,
      .orp_displayed = this->user_orp_displayed_switch_->state,
      .ph_alarm = !this->user_ph_alarm_switch_->state,
      .ph_corrector_alkaline = this->user_ph_corrector_alkaline_switch_->state,
      .ph_control = !this->user_ph_control_switch_->state,
      .cover_installed = this->user_cover_installed_switch_->state,
  };
  this->send_command_(FunctionCode::USER,
                      *reinterpret_cast<char *>(&user_settings));
}

void BSPoolSwitch::send_command_(FunctionCode code, char b2, char b3) {
  this->parent_->write_array({code, b2, b3});
  this->parent_->flush();
}

}  // namespace bs_pool
}  // namespace esphome
