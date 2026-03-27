#include "user_settings.h"

namespace esphome {
namespace bs_pool {

void UserSettingsSwitch::write_state(bool state) {
  this->publish_state(state);
  this->parent_->send_user_state();
}

}  // namespace bs_pool
}  // namespace esphome
