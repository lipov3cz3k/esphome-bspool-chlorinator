#pragma once

#include <cstdint>
#include <string>

namespace esphome {

inline std::string to_string(int val) { return std::to_string(val); }

}

extern uint32_t millis_value;
inline uint32_t millis() { return millis_value; }
