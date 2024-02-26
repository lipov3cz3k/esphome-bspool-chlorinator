#include "bs_pool.h"

#include "esphome/core/log.h"

namespace esphome {
namespace bs_pool {

static const char *const TAG = "bs_pool";

void BSPool::update() {
  for (auto &listener : this->listeners_) {
    for (const FunctionCode code : listener->codes_to_poll()){
      this->write_array({'?', code, '\4'});
      this->flush();
      delay(10);
    }
  }
}

void BSPool::loop() {
  if (!this->available()) return;

  if (this->read_array((uint8_t *)&this->buffer_, sizeof(this->buffer_))) {
    ESP_LOGD(TAG, "Received packet: %02X %02X %02X", this->buffer_.raw[0], this->buffer_.raw[1], this->buffer_.raw[2]);
    for (auto &listener : this->listeners_)
      listener->handle_message(this->buffer_);
  } else {
    ESP_LOGW(TAG, "Junk on wire. Throwing away partial message");
    while (read() >= 0)
      ;
  }
}

}  // namespace bs_pool
}  // namespace esphome
