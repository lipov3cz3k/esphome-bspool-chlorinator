#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace esphome {
namespace uart {

class UARTDevice {
 public:
  virtual ~UARTDevice() = default;

  virtual void write_array(const uint8_t *, size_t) {}

  template<typename T, size_t N>
  void write_array(const std::array<T, N> &data) {
    this->write_array(reinterpret_cast<const uint8_t *>(data.data()), data.size());
  }

  template<typename T>
  void write_array(std::initializer_list<T> data) {
    std::vector<uint8_t> bytes;
    bytes.reserve(data.size());
    for (const auto &item : data) {
      bytes.push_back(static_cast<uint8_t>(item));
    }
    this->write_array(bytes.data(), bytes.size());
  }

  void write_array(std::initializer_list<int> data) {
    std::vector<uint8_t> bytes;
    bytes.reserve(data.size());
    for (int item : data) {
      bytes.push_back(static_cast<uint8_t>(item));
    }
    this->write_array(bytes.data(), bytes.size());
  }

  virtual bool read_array(uint8_t *, size_t) { return false; }
  virtual int available() { return 0; }
  virtual void flush() {}
  virtual bool read_byte(uint8_t *) { return false; }
  virtual int read() { return -1; }
};

}
}
