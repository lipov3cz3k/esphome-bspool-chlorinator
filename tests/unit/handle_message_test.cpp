#include "common.h"
#include "../../components/bs_pool/binary_sensor/bs_pool_binary_sensor.h"
#include "../../components/bs_pool/switch/bs_pool_switch.h"
#include "../../components/bs_pool/select/bs_pool_select.h"
#include "../../components/bs_pool/text_sensor/bs_pool_text_sensor.h"

namespace esphome {
namespace bs_pool {
namespace testing {

TEST(BinarySensorDispatch, AlarmBitmaskBit0) {
  MockBinarySensor alarm_redox;
  BSPoolBinarySensor bs;
  bs.set_alarm_redox_saturated_binary_sensor(&alarm_redox);
  auto pkt = make_packet(FunctionCode::ALARMS, 0x01, 0x00);
  bs.handle_message(pkt);
  EXPECT_TRUE(alarm_redox.published);
  EXPECT_TRUE(alarm_redox.value);
}

TEST(BinarySensorDispatch, AlarmBitmaskBit3) {
  MockBinarySensor alarm_redox, alarm_overtemp, alarm_open, alarm_water, alarm_short,
      alarm_stopped, alarm_comm;
  BSPoolBinarySensor bs;
  bs.set_alarm_redox_saturated_binary_sensor(&alarm_redox);
  bs.set_alarm_overtemperature_binary_sensor(&alarm_overtemp);
  bs.set_alarm_open_circuit_binary_sensor(&alarm_open);
  bs.set_alarm_water_flow_binary_sensor(&alarm_water);
  bs.set_alarm_short_circuit_binary_sensor(&alarm_short);
  bs.set_alarm_unit_stopped_binary_sensor(&alarm_stopped);
  bs.set_alarm_communication_error_binary_sensor(&alarm_comm);
  auto pkt = make_packet(FunctionCode::ALARMS, 0x08, 0x00);
  bs.handle_message(pkt);
  EXPECT_TRUE(alarm_water.published);
  EXPECT_TRUE(alarm_water.value);
  EXPECT_TRUE(alarm_redox.published);
  EXPECT_EQ(alarm_redox.value, false);
  EXPECT_TRUE(alarm_overtemp.published);
  EXPECT_EQ(alarm_overtemp.value, false);
  EXPECT_TRUE(alarm_open.published);
  EXPECT_EQ(alarm_open.value, false);
  EXPECT_TRUE(alarm_short.published);
  EXPECT_EQ(alarm_short.value, false);
  EXPECT_TRUE(alarm_stopped.published);
  EXPECT_EQ(alarm_stopped.value, false);
  EXPECT_TRUE(alarm_comm.published);
  EXPECT_EQ(alarm_comm.value, false);
}

TEST(BinarySensorDispatch, AlarmBitmaskAllClear) {
  MockBinarySensor alarm_redox, alarm_overtemp, alarm_open, alarm_water, alarm_short,
      alarm_stopped, alarm_comm;
  BSPoolBinarySensor bs;
  bs.set_alarm_redox_saturated_binary_sensor(&alarm_redox);
  bs.set_alarm_overtemperature_binary_sensor(&alarm_overtemp);
  bs.set_alarm_open_circuit_binary_sensor(&alarm_open);
  bs.set_alarm_water_flow_binary_sensor(&alarm_water);
  bs.set_alarm_short_circuit_binary_sensor(&alarm_short);
  bs.set_alarm_unit_stopped_binary_sensor(&alarm_stopped);
  bs.set_alarm_communication_error_binary_sensor(&alarm_comm);
  auto pkt = make_packet(FunctionCode::ALARMS, 0x00, 0x00);
  bs.handle_message(pkt);
  EXPECT_TRUE(alarm_redox.published);
  EXPECT_EQ(alarm_redox.value, false);
  EXPECT_TRUE(alarm_overtemp.published);
  EXPECT_EQ(alarm_overtemp.value, false);
  EXPECT_TRUE(alarm_open.published);
  EXPECT_EQ(alarm_open.value, false);
  EXPECT_TRUE(alarm_water.published);
  EXPECT_EQ(alarm_water.value, false);
  EXPECT_TRUE(alarm_short.published);
  EXPECT_EQ(alarm_short.value, false);
  EXPECT_TRUE(alarm_stopped.published);
  EXPECT_EQ(alarm_stopped.value, false);
  EXPECT_TRUE(alarm_comm.published);
  EXPECT_EQ(alarm_comm.value, false);
}

TEST(BinarySensorDispatch, WarningLowSalt) {
  MockBinarySensor low_salt;
  BSPoolBinarySensor bs;
  bs.set_warning_low_salt_binary_sensor(&low_salt);
  auto pkt = make_packet(FunctionCode::WARNINGS, 0x04, 0x00);
  bs.handle_message(pkt);
  EXPECT_TRUE(low_salt.published);
  EXPECT_TRUE(low_salt.value);
}

TEST(BinarySensorDispatch, WarningB3PhPumpOverrun) {
  MockBinarySensor ph_pump;
  BSPoolBinarySensor bs;
  bs.set_warning_ph_pump_overrun_binary_sensor(&ph_pump);
  auto pkt = make_packet(FunctionCode::WARNINGS, 0x00, 0x01);
  bs.handle_message(pkt);
  EXPECT_TRUE(ph_pump.published);
  EXPECT_TRUE(ph_pump.value);
}

TEST(BinarySensorDispatch, StatusFilterPump) {
  MockBinarySensor filter_pump;
  BSPoolBinarySensor bs;
  bs.set_status_filter_pump_running_binary_sensor(&filter_pump);
  auto pkt = make_packet(FunctionCode::STATUS, 0x02, 0x00);
  bs.handle_message(pkt);
  EXPECT_TRUE(filter_pump.published);
  EXPECT_TRUE(filter_pump.value);
}

TEST(BinarySensorDispatch, StatusAllClear) {
  MockBinarySensor polarity, filter, chlor, remote, orp, salt, cleaning, super;
  BSPoolBinarySensor bs;
  bs.set_status_output_voltage_polarity_binary_sensor(&polarity);
  bs.set_status_filter_pump_running_binary_sensor(&filter);
  bs.set_status_chlorination_process_binary_sensor(&chlor);
  bs.set_status_stopped_by_remote_binary_sensor(&remote);
  bs.set_status_orp_sensor_stable_binary_sensor(&orp);
  bs.set_status_salt_sensor_connected_binary_sensor(&salt);
  bs.set_status_electrode_cleaning_binary_sensor(&cleaning);
  bs.set_status_super_chlorination_binary_sensor(&super);
  auto pkt = make_packet(FunctionCode::STATUS, 0x00, 0x00);
  bs.handle_message(pkt);
  EXPECT_TRUE(polarity.published);
  EXPECT_EQ(polarity.value, false);
  EXPECT_TRUE(filter.published);
  EXPECT_EQ(filter.value, false);
  EXPECT_TRUE(chlor.published);
  EXPECT_EQ(chlor.value, false);
  EXPECT_TRUE(remote.published);
  EXPECT_EQ(remote.value, false);
  EXPECT_TRUE(orp.published);
  EXPECT_EQ(orp.value, false);
  EXPECT_TRUE(salt.published);
  EXPECT_EQ(salt.value, false);
  EXPECT_TRUE(cleaning.published);
  EXPECT_EQ(cleaning.value, false);
  EXPECT_TRUE(super.published);
  EXPECT_EQ(super.value, false);
}

TEST(SwitchDispatch, SwitchIsOutdoorInverted_0x00) {
  SwitchSet mocks;
  auto *sw = make_switch_with_all_mocks(mocks);
  auto pkt = make_packet(FunctionCode::USER, 0x00, 0x00);
  sw->handle_message(pkt);
  // bit0=0 with inverted logic → published as true
  EXPECT_TRUE(mocks.user_is_outdoor.published);
  EXPECT_TRUE(mocks.user_is_outdoor.value);
  delete sw;
}

TEST(SwitchDispatch, SwitchIsOutdoorInverted_0xFF) {
  SwitchSet mocks;
  auto *sw = make_switch_with_all_mocks(mocks);
  auto pkt = make_packet(FunctionCode::USER, 0xFF, 0x00);
  sw->handle_message(pkt);
  // bit0=1 with inverted logic → published as false
  EXPECT_TRUE(mocks.user_is_outdoor.published);
  EXPECT_EQ(mocks.user_is_outdoor.value, false);
  delete sw;
}

TEST(SwitchDispatch, SwitchPhControlInverted) {
  SwitchSet mocks;
  auto *sw = make_switch_with_all_mocks(mocks);
  // bit6=1 (0x40) with inverted logic → published as false
  auto pkt = make_packet(FunctionCode::USER, 0x40, 0x00);
  sw->handle_message(pkt);
  EXPECT_TRUE(mocks.user_ph_control.published);
  EXPECT_EQ(mocks.user_ph_control.value, false);
  delete sw;
}

TEST(SelectDispatch, SelectControlModeValid) {
  MockSelect mock_select;
  mock_select.set_options({"Manual", "Automatic", "Semi-automatic"});
  BSPoolSelect sel;
  sel.set_control_mode_select(&mock_select);
  auto pkt = make_packet(FunctionCode::CONTROL_MODE, 0x00, 0x00);
  sel.handle_message(pkt);
  EXPECT_TRUE(mock_select.published);
  EXPECT_EQ(mock_select.value, "Manual");
}

TEST(SelectDispatch, SelectControlModeOutOfRange) {
  MockSelect mock_select;
  mock_select.set_options({"Manual", "Automatic", "Semi-automatic"});
  BSPoolSelect sel;
  sel.set_control_mode_select(&mock_select);
  auto pkt = make_packet(FunctionCode::CONTROL_MODE, 99, 0x00);
  sel.handle_message(pkt);
  EXPECT_EQ(mock_select.published, false);
}

TEST(TextSensorDispatch, TimeFormatHoursMinutes) {
  MockTextSensor mock_time;
  BSPoolTextSensor ts;
  ts.set_time_text_sensor(&mock_time);
  auto pkt = make_packet(FunctionCode::TIME, 30, 14);  // b2=minutes, b3=hours
  ts.handle_message(pkt);
  EXPECT_TRUE(mock_time.published);
  EXPECT_EQ(mock_time.value, "14:30");
}

TEST(TextSensorDispatch, TimeFormatMinutesPadded) {
  MockTextSensor mock_time;
  BSPoolTextSensor ts;
  ts.set_time_text_sensor(&mock_time);
  auto pkt = make_packet(FunctionCode::TIME, 5, 9);  // b2=minutes(<10 → zero-padded), b3=hours
  ts.handle_message(pkt);
  EXPECT_TRUE(mock_time.published);
  EXPECT_EQ(mock_time.value, "9:05");
}

TEST(TextSensorDispatch, VersionFormat) {
  MockTextSensor mock_ver;
  BSPoolTextSensor ts;
  ts.set_version_text_sensor(&mock_ver);
  auto pkt = make_packet(FunctionCode::SOFTWARE_VERSION, 3, 2);  // b2=major, b3=minor
  ts.handle_message(pkt);
  EXPECT_TRUE(mock_ver.published);
  EXPECT_EQ(mock_ver.value, "3.2");
}

TEST(TextSensorDispatch, TimeFormatMidnight) {
  MockTextSensor mock_time;
  BSPoolTextSensor ts;
  ts.set_time_text_sensor(&mock_time);
  auto pkt = make_packet(FunctionCode::TIME, 0, 0);  // b2=minutes, b3=hours
  ts.handle_message(pkt);
  EXPECT_TRUE(mock_time.published);
  EXPECT_EQ(mock_time.value, "0:00");
}

}  // namespace testing
}  // namespace bs_pool
}  // namespace esphome
