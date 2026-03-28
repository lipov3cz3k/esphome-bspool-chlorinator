#include <cstdint>

#include <gtest/gtest.h>

uint32_t millis_value = 0;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
