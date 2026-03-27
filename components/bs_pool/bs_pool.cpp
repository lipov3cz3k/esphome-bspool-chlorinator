#include "bs_pool.h"

#include "esphome/core/log.h"

namespace esphome {
namespace bs_pool {

static const char *const TAG = "bs_pool";

void BSPool::setup() {
  while (this->available())
    this->read();
  ESP_LOGD(TAG, "BSPool UART initialized, RX buffer flushed");
}

void BSPool::update() {
  if (this->queue_size_ != 0)
    return;

  bool seen[256] = {};
  for (auto &listener : this->listeners_) {
    for (const FunctionCode code : listener->codes_to_poll()) {
      const uint8_t code_idx = static_cast<uint8_t>(code);
      if (seen[code_idx])
        continue;
      seen[code_idx] = true;
      if (!this->enqueue_back_({POLL, code, 0, 0})) {
        ESP_LOGW(TAG, "Command queue full, dropping poll command");
        return;
      }
    }
  }
}

void BSPool::enqueue_command(FunctionCode code, uint8_t b2, uint8_t b3) {
  CommandEntry entry{WRITE, code, b2, b3};
  if (!this->enqueue_front_(entry)) {
    ESP_LOGW(TAG, "Command queue full, dropping write command");
  }
}

void BSPool::loop() {
  switch (this->state_) {
    case IDLE: {
      if (this->queue_size_ == 0 && this->available() > 0) {
        ESP_LOGW(TAG, "Unexpected data on UART, discarding");
        while (this->available())
          this->read();
      }

      if (this->queue_size_ > 0 && (millis() - this->last_command_time_) >= INTER_COMMAND_GAP_MS) {
        this->current_command_ = this->dequeue_();
        this->state_ = SENDING;
      }
      break;
    }

    case SENDING: {
      if (this->current_command_.type == POLL) {
        this->write_array({'?', this->current_command_.code, '\4'});
      } else {
        this->write_array({this->current_command_.code, this->current_command_.b2, this->current_command_.b3});
      }
      this->flush();
      this->last_command_time_ = millis();
      this->state_ = WAITING;
      break;
    }

    case WAITING: {
      if ((millis() - this->last_command_time_) >= RESPONSE_TIMEOUT_MS) {
        ESP_LOGW(TAG, "Response timeout for command 0x%02X", static_cast<uint8_t>(this->current_command_.code));
        while (this->available())
          this->read();
        this->state_ = IDLE;
        return;
      }

      if (this->available() >= 3) {
        if (this->read_array((uint8_t *) &this->buffer_, sizeof(this->buffer_))) {
          if (this->buffer_.raw[0] == 0x45 && this->buffer_.raw[1] == 0x52) {
            ESP_LOGW(TAG, "Device error response: code=%d", this->buffer_.raw[2]);
          } else {
            ESP_LOGD(TAG, "Received packet: %02X %02X %02X", this->buffer_.raw[0], this->buffer_.raw[1], this->buffer_.raw[2]);
            for (auto &listener : this->listeners_)
              listener->handle_message(this->buffer_);
          }
        } else {
          ESP_LOGW(TAG, "Junk on wire. Throwing away partial message");
          while (this->available())
            this->read();
        }
        this->state_ = IDLE;
      }

      break;
    }
  }
}

bool BSPool::enqueue_back_(CommandEntry entry) {
  if (this->queue_size_ >= MAX_QUEUE_SIZE)
    return false;
  this->queue_[(this->queue_head_ + this->queue_size_) % MAX_QUEUE_SIZE] = entry;
  this->queue_size_++;
  return true;
}

bool BSPool::enqueue_front_(CommandEntry entry) {
  if (this->queue_size_ >= MAX_QUEUE_SIZE)
    return false;
  this->queue_head_ = (this->queue_head_ + MAX_QUEUE_SIZE - 1) % MAX_QUEUE_SIZE;
  this->queue_[this->queue_head_] = entry;
  this->queue_size_++;
  return true;
}

CommandEntry BSPool::dequeue_() {
  CommandEntry entry = this->queue_[this->queue_head_];
  this->queue_head_ = (this->queue_head_ + 1) % MAX_QUEUE_SIZE;
  this->queue_size_--;
  return entry;
}

}  // namespace bs_pool
}  // namespace esphome
