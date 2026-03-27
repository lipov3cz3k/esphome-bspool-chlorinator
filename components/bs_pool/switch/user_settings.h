#pragma once

#include "bs_pool_switch.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace bs_pool {


class UserSettingsSwitch : public switch_::Switch, public Parented<BSPoolSwitch> {
 protected:
  void write_state(bool state) override;
};

}  // namespace bs_pool
}  // namespace esphome
