#include <deque>
#include <vector>

#include "common.h"
#include "../../components/bs_pool/select/bs_pool_select.h"
#include "../../components/bs_pool/switch/bs_pool_switch.h"

namespace esphome {
namespace bs_pool {
namespace testing {

class TestableBSPool : public BSPool {
 public:
  std::vector<std::vector<uint8_t>> written_data;
  int flush_count{0};
  std::deque<uint8_t> rx_buffer;
  bool force_read_array_fail{false};

  void write_array(const uint8_t *data, size_t len) override {
    written_data.emplace_back(data, data + len);
  }

  int available() override { return static_cast<int>(rx_buffer.size()); }

  bool read_array(uint8_t *data, size_t len) override {
    if (force_read_array_fail) {
      return false;
    }
    if (rx_buffer.size() < len) {
      return false;
    }
    for (size_t i = 0; i < len; i++) {
      data[i] = rx_buffer.front();
      rx_buffer.pop_front();
    }
    return true;
  }

  int read() override {
    if (rx_buffer.empty()) {
      return -1;
    }
    int out = rx_buffer.front();
    rx_buffer.pop_front();
    return out;
  }

  void flush() override { flush_count++; }

  void inject_rx(uint8_t b1, uint8_t b2, uint8_t b3) {
    rx_buffer.push_back(b1);
    rx_buffer.push_back(b2);
    rx_buffer.push_back(b3);
  }

  void clear() {
    written_data.clear();
    flush_count = 0;
    rx_buffer.clear();
    force_read_array_fail = false;
  }
};

class MockListener : public BSPoolListener {
 public:
  std::vector<FunctionCode> poll_codes;
  std::vector<DataPacket> received;

  const std::vector<FunctionCode> codes_to_poll() override { return poll_codes; }

  void handle_message(DataPacket &message) override { received.push_back(message); }
};

TEST(Hub, UpdateNoListeners) {
  TestableBSPool hub;
  hub.update();

  EXPECT_TRUE(hub.written_data.empty());
  EXPECT_EQ(hub.flush_count, 0);
}

TEST(Hub, UpdateSingleListenerSingleCode) {
  TestableBSPool hub;
  MockListener l1;
  l1.poll_codes = {FunctionCode::ALARMS};
  hub.register_listener(&l1);

  hub.update();

  ASSERT_EQ(hub.written_data.size(), 1U);
  EXPECT_EQ(hub.written_data[0], (std::vector<uint8_t>{'?', static_cast<uint8_t>(FunctionCode::ALARMS), '\4'}));
  EXPECT_EQ(hub.flush_count, 1);
}

TEST(Hub, UpdateSingleListenerMultipleCodes) {
  TestableBSPool hub;
  MockListener l1;
  l1.poll_codes = {FunctionCode::ALARMS, FunctionCode::STATUS, FunctionCode::WARNINGS};
  hub.register_listener(&l1);

  hub.update();

  ASSERT_EQ(hub.written_data.size(), 3U);
  EXPECT_EQ(hub.written_data[0], (std::vector<uint8_t>{'?', static_cast<uint8_t>(FunctionCode::ALARMS), '\4'}));
  EXPECT_EQ(hub.written_data[1], (std::vector<uint8_t>{'?', static_cast<uint8_t>(FunctionCode::STATUS), '\4'}));
  EXPECT_EQ(hub.written_data[2], (std::vector<uint8_t>{'?', static_cast<uint8_t>(FunctionCode::WARNINGS), '\4'}));
  EXPECT_EQ(hub.flush_count, 3);
}

TEST(Hub, UpdateMultipleListeners) {
  TestableBSPool hub;
  MockListener l1;
  MockListener l2;
  l1.poll_codes = {FunctionCode::ALARMS, FunctionCode::STATUS};
  l2.poll_codes = {FunctionCode::USER};
  hub.register_listener(&l1);
  hub.register_listener(&l2);

  hub.update();

  ASSERT_EQ(hub.written_data.size(), 3U);
  EXPECT_EQ(hub.written_data[0], (std::vector<uint8_t>{'?', static_cast<uint8_t>(FunctionCode::ALARMS), '\4'}));
  EXPECT_EQ(hub.written_data[1], (std::vector<uint8_t>{'?', static_cast<uint8_t>(FunctionCode::STATUS), '\4'}));
  EXPECT_EQ(hub.written_data[2], (std::vector<uint8_t>{'?', static_cast<uint8_t>(FunctionCode::USER), '\4'}));
  EXPECT_EQ(hub.flush_count, 3);
}

TEST(Hub, LoopNoData) {
  TestableBSPool hub;
  MockListener l1;
  hub.register_listener(&l1);

  hub.loop();

  EXPECT_TRUE(l1.received.empty());
  EXPECT_TRUE(hub.rx_buffer.empty());
}

TEST(Hub, LoopValidPacket) {
  TestableBSPool hub;
  MockListener l1;
  hub.register_listener(&l1);
  hub.inject_rx(0x41, 0x0F, 0x00);

  hub.loop();

  ASSERT_EQ(l1.received.size(), 1U);
  EXPECT_EQ(l1.received[0].function_code, 0x41);
  EXPECT_EQ(l1.received[0].data_b2, 0x0F);
  EXPECT_EQ(l1.received[0].data_b3, 0x00);
}

TEST(Hub, LoopMultipleListeners) {
  TestableBSPool hub;
  MockListener l1;
  MockListener l2;
  hub.register_listener(&l1);
  hub.register_listener(&l2);
  hub.inject_rx(static_cast<uint8_t>(FunctionCode::STATUS), 0xAA, 0x55);

  hub.loop();

  ASSERT_EQ(l1.received.size(), 1U);
  ASSERT_EQ(l2.received.size(), 1U);
  EXPECT_EQ(l1.received[0].function_code, static_cast<uint8_t>(FunctionCode::STATUS));
  EXPECT_EQ(l2.received[0].function_code, static_cast<uint8_t>(FunctionCode::STATUS));
  EXPECT_EQ(l1.received[0].data_b2, 0xAA);
  EXPECT_EQ(l2.received[0].data_b2, 0xAA);
  EXPECT_EQ(l1.received[0].data_b3, 0x55);
  EXPECT_EQ(l2.received[0].data_b3, 0x55);
}

TEST(Hub, LoopPartialRead) {
  TestableBSPool hub;
  MockListener l1;
  hub.register_listener(&l1);
  hub.inject_rx(0x41, 0x0F, 0x00);
  hub.force_read_array_fail = true;

  hub.loop();

  EXPECT_TRUE(l1.received.empty());
  EXPECT_TRUE(hub.rx_buffer.empty());
}

TEST(Hub, RegisterListener) {
  TestableBSPool hub;
  MockListener l1;
  l1.poll_codes = {FunctionCode::WARNINGS};

  hub.register_listener(&l1);
  hub.update();

  ASSERT_EQ(hub.written_data.size(), 1U);
  EXPECT_EQ(hub.written_data[0], (std::vector<uint8_t>{'?', static_cast<uint8_t>(FunctionCode::WARNINGS), '\4'}));
}

TEST(Hub, SwitchSendUserState) {
  TestableBSPool parent;
  SwitchSet mocks;
  auto *sw = make_switch_with_all_mocks(mocks);
  sw->set_parent(&parent);

  mocks.user_is_outdoor.state = true;             // bit0 inverted -> 1
  mocks.user_cover_switch_off.state = false;      // bit1 -> 0
  mocks.user_flow_switch_installed.state = true;  // bit2 -> 1
  mocks.user_orp_displayed.state = false;         // bit3 -> 0
  mocks.user_ph_alarm.state = true;               // bit4 inverted -> 0
  mocks.user_ph_corrector_alkaline.state = true;  // bit5 -> 1
  mocks.user_ph_control.state = false;            // bit6 inverted -> 1
  mocks.user_cover_installed.state = true;        // bit7 -> 1
  // binary: 1110_0101 = 0xE5

  sw->send_user_state();

  ASSERT_EQ(parent.written_data.size(), 1U);
  ASSERT_EQ(parent.written_data[0].size(), 3U);
  EXPECT_EQ(parent.written_data[0][0], static_cast<uint8_t>(FunctionCode::USER));
  EXPECT_EQ(parent.written_data[0][1], static_cast<uint8_t>(0xE5));
  EXPECT_EQ(parent.written_data[0][2], static_cast<uint8_t>('\4'));
  EXPECT_EQ(parent.flush_count, 1);

  delete sw;
}

TEST(Hub, SelectSendCommandWriteBack) {
  TestableBSPool parent;
  BSPoolSelect sel;
  sel.set_parent(&parent);

  sel.send_command(FunctionCode::CONTROL_MODE, 0x02);

  ASSERT_EQ(parent.written_data.size(), 1U);
  EXPECT_EQ(parent.written_data[0], (std::vector<uint8_t>{
                                       static_cast<uint8_t>(FunctionCode::CONTROL_MODE),
                                       static_cast<uint8_t>(0x02),
                                       static_cast<uint8_t>('\4'),
                                     }));
  EXPECT_EQ(parent.flush_count, 1);
}

}  // namespace testing
}  // namespace bs_pool
}  // namespace esphome
