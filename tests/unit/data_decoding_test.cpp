#include <cmath>

#include "common.h"
#include "../../components/bs_pool/sensor/bs_pool_sensor.h"

namespace esphome {
namespace bs_pool {
namespace testing {

TEST(SensorDecoding, CellCurrent) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_cell_current_sensor(&mock);
  auto pkt = make_packet(FunctionCode::CELL_CURRENT_MEASUREMENT, 0xC8, 0x00);
  sensor.handle_message(pkt);
  EXPECT_FLOAT_EQ(mock.value, 20.0f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, CellCurrentLarge) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_cell_current_sensor(&mock);
  auto pkt = make_packet(FunctionCode::CELL_CURRENT_MEASUREMENT, 0xD0, 0x07);
  sensor.handle_message(pkt);
  EXPECT_FLOAT_EQ(mock.value, 200.0f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, CellIntensity) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_cell_intensity_sensor(&mock);
  auto pkt = make_packet(FunctionCode::CELL_INTENSITY_MEASUREMENT, 0x50, 0x00);
  sensor.handle_message(pkt);
  EXPECT_FLOAT_EQ(mock.value, 80.0f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, CellVoltage) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_cell_voltage_sensor(&mock);
  auto pkt = make_packet(FunctionCode::CELL_VOLTAGE_MEASUREMENT, 0xE8, 0x03);
  sensor.handle_message(pkt);
  EXPECT_FLOAT_EQ(mock.value, 100.0f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, PhValid) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_ph_sensor(&mock);
  auto pkt = make_packet(FunctionCode::PH_MEASUREMENT, 0xD4, 0x02);
  sensor.handle_message(pkt);
  EXPECT_NEAR(mock.value, 7.24f, 0.001f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, PhInvalid) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_ph_sensor(&mock);
  auto pkt = make_packet(FunctionCode::PH_MEASUREMENT, 0xFF, 0xFF);
  sensor.handle_message(pkt);
  EXPECT_TRUE(std::isnan(mock.value));
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, SaltValid) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_salt_concentration_sensor(&mock);
  auto pkt = make_packet(FunctionCode::SALT_CONCENTRATION_MEASUREMENT, 0x58, 0x02);
  sensor.handle_message(pkt);
  EXPECT_NEAR(mock.value, 6.0f, 0.001f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, SaltInvalid) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_salt_concentration_sensor(&mock);
  auto pkt = make_packet(FunctionCode::SALT_CONCENTRATION_MEASUREMENT, 0xFF, 0xFF);
  sensor.handle_message(pkt);
  EXPECT_TRUE(std::isnan(mock.value));
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, Radox) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_radox_sensor(&mock);
  auto pkt = make_packet(FunctionCode::RADOX_MEASUREMENT, 0x2C, 0x01);
  sensor.handle_message(pkt);
  EXPECT_FLOAT_EQ(mock.value, 300.0f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, TemperaturePositive) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_temperature_sensor(&mock);
  auto pkt = make_packet(FunctionCode::TEMPERATURE_MEASUREMENT, 0x1B, 0x00);
  sensor.handle_message(pkt);
  EXPECT_FLOAT_EQ(mock.value, 27.0f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, TemperatureNegative) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_temperature_sensor(&mock);
  auto pkt = make_packet(FunctionCode::TEMPERATURE_MEASUREMENT, 0x05, 0x01);
  sensor.handle_message(pkt);
  EXPECT_FLOAT_EQ(mock.value, -5.0f);
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, TemperatureNan) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_temperature_sensor(&mock);
  auto pkt = make_packet(FunctionCode::TEMPERATURE_MEASUREMENT, 0xFF, 0x01);
  sensor.handle_message(pkt);
  EXPECT_TRUE(std::isnan(mock.value));
  EXPECT_TRUE(mock.published);
}

TEST(SensorDecoding, TemperaturePositiveLarge) {
  MockSensor mock;
  BSPoolSensor sensor;
  sensor.set_temperature_sensor(&mock);
  auto pkt = make_packet(FunctionCode::TEMPERATURE_MEASUREMENT, 0xFF, 0x00);
  sensor.handle_message(pkt);
  EXPECT_FLOAT_EQ(mock.value, 255.0f);
  EXPECT_TRUE(mock.published);
}

}  // namespace testing
}  // namespace bs_pool
}  // namespace esphome
