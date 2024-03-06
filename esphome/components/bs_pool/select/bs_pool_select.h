#pragma once

#include "../bs_pool.h"
#include "esphome/components/select/select.h"

namespace esphome {
namespace bs_pool {

class BSPoolSelect : public BSPoolListener,
                     public Component,
                     public Parented<BSPool> {
 public:
  void dump_config() override;

  const std::vector<FunctionCode> codes_to_poll() override;
  void handle_message(DataPacket &message) override;

  void send_command(FunctionCode code, char b2, char b3='\4');

  SUB_SELECT(control_mode);
};

}  // namespace bs_pool
}  // namespace esphome
