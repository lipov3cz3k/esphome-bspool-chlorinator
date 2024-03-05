#pragma once

#include "../bs_pool.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace bs_pool {

class BSPoolSensor : public BSPoolListener, public Component {
 public:
  void dump_config() override;

  const std::vector<FunctionCode> codes_to_poll() override;
  void handle_message(DataPacket &message) override;

  SUB_SENSOR(cell_current);
  SUB_SENSOR(cell_intensity);
  SUB_SENSOR(cell_voltage);
  SUB_SENSOR(ph);
  SUB_SENSOR(salt_concentration);
  SUB_SENSOR(time);
  SUB_SENSOR(version);
  SUB_SENSOR(radox);
};
}  // namespace bs_pool
}  // namespace esphome
