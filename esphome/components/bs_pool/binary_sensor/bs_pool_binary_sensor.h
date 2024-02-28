#pragma once

#include "../bs_pool.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace bs_pool {

class BSPoolBinarySensor : public BSPoolListener, public Component {
 public:
  void dump_config() override;

  const std::vector<FunctionCode> codes_to_poll() override;
  void handle_message(DataPacket &message) override;

  SUB_BINARY_SENSOR(alarm_redox_saturated);
  SUB_BINARY_SENSOR(alarm_overtemperature);
  SUB_BINARY_SENSOR(alarm_open_circuit);
  SUB_BINARY_SENSOR(alarm_water_flow);
  SUB_BINARY_SENSOR(alarm_short_circuit);
  SUB_BINARY_SENSOR(alarm_unit_stopped);
  SUB_BINARY_SENSOR(alarm_communication_error);

  SUB_BINARY_SENSOR(warning_low_salt);
  SUB_BINARY_SENSOR(warning_high_salt);
  SUB_BINARY_SENSOR(warning_ph_pump_overrun);
  SUB_BINARY_SENSOR(warning_acid);
  SUB_BINARY_SENSOR(warning_clock_set);
  SUB_BINARY_SENSOR(warning_probe_disconnected);

  SUB_BINARY_SENSOR(status_output_voltage_polarity);
  SUB_BINARY_SENSOR(status_filter_pump_running);
  SUB_BINARY_SENSOR(status_chlorination_process);
  SUB_BINARY_SENSOR(status_stopped_by_remote);
  SUB_BINARY_SENSOR(status_orp_sensor_stable);
  SUB_BINARY_SENSOR(status_salt_sensor_connected);
  SUB_BINARY_SENSOR(status_electrode_cleaning);
  SUB_BINARY_SENSOR(status_super_chlorination);
};
}  // namespace bs_pool
}  // namespace esphome
