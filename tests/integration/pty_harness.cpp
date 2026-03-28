#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "../../components/bs_pool/bs_pool.h"
#include "../../components/bs_pool/sensor/bs_pool_sensor.h"

namespace esphome {
namespace bs_pool {

class PtyUARTDevice : public uart::UARTDevice {
 public:
  int fd{-1};

  void write_array(const uint8_t *data, size_t len) override {
    ssize_t written = write(fd, data, len);
    (void)written;
  }

  bool read_array(uint8_t *data, size_t len) override {
    size_t got = 0;
    while (got < len) {
      ssize_t n = ::read(fd, data + got, len - got);
      if (n <= 0) return false;
      got += static_cast<size_t>(n);
    }
    return true;
  }

  int available() override {
    int bytes = 0;
    ioctl(fd, FIONREAD, &bytes);
    return bytes;
  }

  void flush() override { tcdrain(fd); }

  int read() override {
    uint8_t b;
    ssize_t n = ::read(fd, &b, 1);
    return n == 1 ? b : -1;
  }
};

class RecordingBSPool : public BSPool {
 public:
  PtyUARTDevice uart_dev;

  void write_array(const uint8_t *data, size_t len) override { uart_dev.write_array(data, len); }
  bool read_array(uint8_t *data, size_t len) override { return uart_dev.read_array(data, len); }
  int available() override { return uart_dev.available(); }
  void flush() override { uart_dev.flush(); }
  int read() override { return uart_dev.read(); }
};

class RecordingListener : public BSPoolListener {
 public:
  bool received{false};
  float cell_current{0.0f};

  const std::vector<FunctionCode> codes_to_poll() override { return {FunctionCode::CELL_CURRENT_MEASUREMENT}; }

  void handle_message(DataPacket &pkt) override {
    if (pkt.function_code == static_cast<uint8_t>(FunctionCode::CELL_CURRENT_MEASUREMENT)) {
      uint16_t raw = (static_cast<uint16_t>(pkt.data_b3) << 8) | pkt.data_b2;
      cell_current = raw / 10.0f;
      received = true;
    }
  }
};

}  // namespace bs_pool
}  // namespace esphome

static void configure_tty(int fd) {
  struct termios tty;
  memset(&tty, 0, sizeof(tty));
  cfsetispeed(&tty, B19200);
  cfsetospeed(&tty, B19200);
  tty.c_cflag |= (CLOCAL | CREAD | CS8);
  tty.c_iflag = IGNPAR;
  tty.c_oflag = 0;
  tty.c_lflag = 0;
  tty.c_cc[VMIN] = 1;
  tty.c_cc[VTIME] = 0;
  tcsetattr(fd, TCSANOW, &tty);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <slave_pty_path>\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
  configure_tty(fd);

  using namespace esphome::bs_pool;

  RecordingBSPool hub;
  hub.uart_dev.fd = fd;

  RecordingListener listener;
  hub.register_listener(&listener);

  hub.update();

  usleep(100000);

  struct timeval tv;
  tv.tv_sec = 3;
  tv.tv_usec = 0;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  int ready = select(fd + 1, &fds, nullptr, nullptr, &tv);
  if (ready <= 0) {
    fprintf(stderr, "FAIL: timeout waiting for response\n");
    close(fd);
    return 1;
  }

  hub.loop();

  close(fd);

  if (!listener.received) {
    fprintf(stderr, "FAIL: listener never received a packet\n");
    return 1;
  }

  if (listener.cell_current < 19.9f || listener.cell_current > 20.1f) {
    fprintf(stderr, "FAIL: unexpected cell_current=%.2f (expected 20.0)\n", listener.cell_current);
    return 1;
  }

  printf("OK: cell_current=%.1f\n", listener.cell_current);
  return 0;
}
