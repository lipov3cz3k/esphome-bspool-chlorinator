#pragma once

#include "../bs_pool.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace bs_pool {

class BSPoolSwitch : public BSPoolListener,
                     public Component,
                     public Parented<BSPool> {
 public:
  void dump_config() override;

  const std::vector<FunctionCode> codes_to_poll() override;
  void handle_message(DataPacket &message) override;

  // User settings
  void send_user_state();
  SUB_SWITCH(user_is_outdoor);
  SUB_SWITCH(user_cover_switch_off);
  SUB_SWITCH(user_flow_switch_installed);
  SUB_SWITCH(user_orp_displayed);
  SUB_SWITCH(user_ph_alarm);
  SUB_SWITCH(user_ph_corrector_alkaline);
  SUB_SWITCH(user_ph_control);
  SUB_SWITCH(user_cover_installed);

 private:
  uint8_t last_user_byte_{0xFF};
  bool has_user_readback_{false};
};

}  // namespace bs_pool
}  // namespace esphome
