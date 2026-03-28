#include <cstdint>

#include <gtest/gtest.h>

#include "../../components/bs_pool/bs_pool.h"

namespace esphome {
namespace bs_pool {
namespace testing {

TEST(BSPoolSmokeTest, DataPacketUnionLayout) {
  DataPacket p;
  p.function_code = 'c';
  p.data_b2 = 0xC8;
  p.data_b3 = 0x00;
  EXPECT_EQ(p.raw[0], static_cast<uint8_t>('c'));
  EXPECT_EQ(p.raw[1], static_cast<uint8_t>(0xC8));
  EXPECT_EQ(p.raw[2], static_cast<uint8_t>(0x00));
}

TEST(BSPoolSmokeTest, FunctionCodeEnum) {
  EXPECT_EQ(static_cast<char>(FunctionCode::CELL_CURRENT_MEASUREMENT), 'c');
  EXPECT_EQ(static_cast<char>(FunctionCode::PH_MEASUREMENT), 'p');
}

}
}
}
