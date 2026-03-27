#pragma once

#include "../bs_pool.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace bs_pool {

class BSPoolTextSensor : public BSPoolListener, public Component {
 public:
  void dump_config() override;

  const std::vector<FunctionCode> codes_to_poll() override;
  void handle_message(DataPacket &message) override;

  SUB_TEXT_SENSOR(version)
  SUB_TEXT_SENSOR(time)
};
}  // namespace bs_pool
}  // namespace esphome
