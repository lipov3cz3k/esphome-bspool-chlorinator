#!/usr/bin/env python3
"""PTY integration smoke test for BSPool chlorinator component.

Creates a PTY pair, spawns the pty_harness binary against the slave end,
and acts as the chlorinator simulator on the master end.

Round-trip: BSPool sends poll {'?', 'c', '\4'} → simulator reads it →
simulator responds {'c', 0xC8, 0x00} → BSPool dispatches to listener.

Exit 0 = PASS, non-zero = FAIL.
"""

import os
import subprocess
import sys
import time

HARNESS_BINARY = os.path.join(os.path.dirname(__file__), "../../build/pty_harness")


def run():
    try:
        import pty  # noqa: F401 — check PTY availability
    except ImportError:
        print("SKIP: PTY not available on this platform")
        sys.exit(0)

    if not os.path.exists(HARNESS_BINARY):
        print(f"SKIP: harness binary not found at {HARNESS_BINARY}")
        print("  Build with: cmake -B build tests/unit && cmake --build build")
        sys.exit(0)

    import pty

    master_fd, slave_fd = pty.openpty()
    slave_path = os.ttyname(slave_fd)

    try:
        proc = subprocess.Popen(
            [HARNESS_BINARY, slave_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )

        os.close(slave_fd)

        # Wait for the poll request: {'?', 'c', '\4'}
        os.set_blocking(master_fd, False)
        deadline = time.monotonic() + 3.0
        buf = b""
        while len(buf) < 3 and time.monotonic() < deadline:
            try:
                buf += os.read(master_fd, 3 - len(buf))
            except BlockingIOError:
                time.sleep(0.01)

        if len(buf) < 3:
            print(f"FAIL: timeout waiting for poll request (got {buf!r})")
            proc.kill()
            sys.exit(1)

        expected_req = bytes([ord("?"), ord("c"), 0x04])
        if buf != expected_req:
            print(f"FAIL: unexpected poll request: {buf!r} (expected {expected_req!r})")
            proc.kill()
            sys.exit(1)

        print(f"OK: received poll request {buf!r}")

        # Write chlorinator response: cell_current = 20.0A → raw 200 = 0x00C8, B2=0xC8 B3=0x00
        response = bytes([ord("c"), 0xC8, 0x00])
        os.write(master_fd, response)
        print(f"OK: sent response {response!r}")

        try:
            proc.wait(timeout=3.0)
        except subprocess.TimeoutExpired:
            print("FAIL: harness did not exit within 3 seconds")
            proc.kill()
            sys.exit(1)

        if proc.returncode != 0:
            stdout = proc.stdout.read().decode(errors="replace")
            stderr = proc.stderr.read().decode(errors="replace")
            print(f"FAIL: harness exited with code {proc.returncode}")
            if stdout:
                print(f"  stdout: {stdout}")
            if stderr:
                print(f"  stderr: {stderr}")
            sys.exit(1)

        print("PASS: PTY round-trip smoke test complete")
        sys.exit(0)

    finally:
        try:
            os.close(master_fd)
        except OSError:
            pass


if __name__ == "__main__":
    run()
