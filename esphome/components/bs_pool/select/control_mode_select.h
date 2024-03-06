#pragma once

#include "bs_pool_select.h"
#include "esphome/components/select/select.h"

namespace esphome {
namespace bs_pool {

class ControlModeSelect : public select::Select, public Parented<BSPoolSelect> {
 protected:
  void control(const std::string &value) override;
};

}  // namespace bs_pool
}  // namespace esphome
