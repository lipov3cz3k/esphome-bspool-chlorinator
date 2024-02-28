#include "bs_pool_binary_sensor.h"

#include "esphome/core/log.h"

namespace esphome {
namespace bs_pool {

static const char *const TAG = "bs_pool.binary_sensor";

void BSPoolBinarySensor::dump_config() { ESP_LOGCONFIG(TAG, "BSPool:"); }

const std::vector<FunctionCode> BSPoolBinarySensor::codes_to_poll() {
  std::vector<FunctionCode> active_codes;
  // Ask for the alarms if any of sensors are set
  if (this->alarm_redox_saturated_binary_sensor_ != nullptr ||
      this->alarm_overtemperature_binary_sensor_ != nullptr ||
      this->alarm_open_circuit_binary_sensor_ != nullptr ||
      this->alarm_water_flow_binary_sensor_ != nullptr ||
      this->alarm_short_circuit_binary_sensor_ != nullptr ||
      this->alarm_unit_stopped_binary_sensor_ != nullptr ||
      this->alarm_communication_error_binary_sensor_ != nullptr)
    active_codes.push_back(FunctionCode::ALARMS);

  // Ask for the warnings if any of sensors are set
  if (this->warning_low_salt_binary_sensor_ != nullptr ||
      this->warning_high_salt_binary_sensor_ != nullptr ||
      this->warning_ph_pump_overrun_binary_sensor_ != nullptr ||
      this->warning_acid_binary_sensor_ != nullptr ||
      this->warning_clock_set_binary_sensor_ != nullptr ||
      this->warning_probe_disconnected_binary_sensor_ != nullptr)
    active_codes.push_back(FunctionCode::WARNINGS);

  if (this->status_output_voltage_polarity_binary_sensor_ != nullptr ||
      this->status_filter_pump_running_binary_sensor_ != nullptr ||
      this->status_chlorination_process_binary_sensor_ != nullptr ||
      this->status_stopped_by_remote_binary_sensor_ != nullptr ||
      this->status_orp_sensor_stable_binary_sensor_ != nullptr ||
      this->status_salt_sensor_connected_binary_sensor_ != nullptr ||
      this->status_electrode_cleaning_binary_sensor_ != nullptr ||
      this->status_super_chlorination_binary_sensor_ != nullptr)
    active_codes.push_back(FunctionCode::STATUS);

  return active_codes;
}

void BSPoolBinarySensor::handle_message(DataPacket &message) {
  switch (message.function_code) {
    case FunctionCode::ALARMS:
      if (this->alarm_redox_saturated_binary_sensor_ != nullptr)
        this->alarm_redox_saturated_binary_sensor_->publish_state(message.data_b2 & 0x01);
      if (this->alarm_overtemperature_binary_sensor_ != nullptr)
        this->alarm_overtemperature_binary_sensor_->publish_state(message.data_b2 & 0x02);
      if (this->alarm_open_circuit_binary_sensor_ != nullptr)
        this->alarm_open_circuit_binary_sensor_->publish_state(message.data_b2 & 0x04);
      if (this->alarm_water_flow_binary_sensor_ != nullptr)
        this->alarm_water_flow_binary_sensor_->publish_state(message.data_b2 & 0x08);
      if (this->alarm_short_circuit_binary_sensor_ != nullptr)
        this->alarm_short_circuit_binary_sensor_->publish_state(message.data_b2 & 0x10);
      if (this->alarm_unit_stopped_binary_sensor_ != nullptr)
        this->alarm_unit_stopped_binary_sensor_->publish_state(message.data_b2 & 0x20);
      if (this->alarm_communication_error_binary_sensor_ != nullptr)
        this->alarm_communication_error_binary_sensor_->publish_state(message.data_b2 & 0x40);
      break;

    case FunctionCode::WARNINGS:
      if (this->warning_low_salt_binary_sensor_ != nullptr)
        this->warning_low_salt_binary_sensor_->publish_state(message.data_b2 & 0x04);
      if (this->warning_high_salt_binary_sensor_ != nullptr)
        this->warning_high_salt_binary_sensor_->publish_state(message.data_b2 & 0x08);
      if (this->warning_ph_pump_overrun_binary_sensor_ != nullptr)
        this->warning_ph_pump_overrun_binary_sensor_->publish_state(message.data_b3 & 0x01);
      if (this->warning_acid_binary_sensor_ != nullptr)
        this->warning_acid_binary_sensor_->publish_state(message.data_b3 & 0x02);
      if (this->warning_clock_set_binary_sensor_ != nullptr)
        this->warning_clock_set_binary_sensor_->publish_state(message.data_b3 & 0x04);
      if (this->warning_probe_disconnected_binary_sensor_ != nullptr)
        this->warning_probe_disconnected_binary_sensor_->publish_state(message.data_b3 & 0x08);
      break;

    case FunctionCode::STATUS:
      if (this->status_output_voltage_polarity_binary_sensor_ != nullptr)
        this->status_output_voltage_polarity_binary_sensor_->publish_state(message.data_b2 & 0x01);
      if (this->status_filter_pump_running_binary_sensor_ != nullptr)
        this->status_filter_pump_running_binary_sensor_->publish_state(message.data_b2 & 0x02);
      if (this->status_chlorination_process_binary_sensor_ != nullptr)
        this->status_chlorination_process_binary_sensor_->publish_state(message.data_b2 & 0x04);
      if (this->status_stopped_by_remote_binary_sensor_ != nullptr)
        this->status_stopped_by_remote_binary_sensor_->publish_state(message.data_b2 & 0x08);
      if (this->status_orp_sensor_stable_binary_sensor_ != nullptr)
        this->status_orp_sensor_stable_binary_sensor_->publish_state(message.data_b2 & 0x10);
      if (this->status_salt_sensor_connected_binary_sensor_ != nullptr)
        this->status_salt_sensor_connected_binary_sensor_->publish_state(message.data_b2 & 0x20);
      if (this->status_electrode_cleaning_binary_sensor_ != nullptr)
        this->status_electrode_cleaning_binary_sensor_->publish_state(message.data_b2 & 0x40);
      if (this->status_super_chlorination_binary_sensor_ != nullptr)
        this->status_super_chlorination_binary_sensor_->publish_state(message.data_b2 & 0x80);
      break;
  }
}

}  // namespace bs_pool
}  // namespace esphome
