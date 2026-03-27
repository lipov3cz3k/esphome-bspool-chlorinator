#pragma once

#include "../bs_pool.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace bs_pool {

struct UserSettings {
    bool is_outdoor : 1;
    bool cover_switch_off : 1;
    bool flow_switch_installed : 1;
    bool orp_displayed : 1;
    bool ph_alarm : 1;
    bool ph_corrector_alkaline : 1;
    bool ph_control : 1;
    bool cover_installed : 1;
};

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
  void send_command_(FunctionCode code, char b2, char b3 = '\4');
};

}  // namespace bs_pool
}  // namespace esphome
