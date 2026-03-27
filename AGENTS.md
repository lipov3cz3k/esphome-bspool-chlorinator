# esphome-bspool-chlorinator

ESPHome external component for **BSPool VA Salt (Smart)** salt-water chlorinator. Communicates over UART (RS232-TTL) using a proprietary 3-byte request/response protocol.

## Quick Start

```bash
# Build test (ESP8266, Arduino framework)
# Requires ESPHome CLI installed
esphome compile tests/components/bs_pool/test.esp8266-ard.yaml

# Usage: add as external component in your ESPHome YAML
# See example.yaml for full configuration
```

## Architecture

```
components/bs_pool/                # All source code lives here
├── __init__.py + bs_pool.{h,cpp}  # Core: BSPool hub, BSPoolListener, DataPacket, FunctionCode
├── sensor/                        # 7 measurement sensors
├── binary_sensor/                 # 21 status/alarm/warning binary sensors
├── switch/                        # 8 user settings switches
├── select/                        # 1 control mode select
└── text_sensor/                   # 2 text sensors (time, version)

tests/components/bs_pool/          # Build tests (ESP8266)
PCB/                               # Hardware: schematics, BOM, wiring (read-only reference)
```

### Core Pattern

**Listener-based polling**: `BSPool` (PollingComponent + UARTDevice) polls the chlorinator every 60s. Each sub-component registers as a `BSPoolListener` and declares which `FunctionCode`s it needs via `codes_to_poll()`. Responses are dispatched to all listeners via `handle_message(DataPacket&)`.

### Sub-component Structure (repeated 5×)

Every entity type (sensor, binary_sensor, switch, select, text_sensor) follows the same pattern:

| File | Purpose |
|------|---------|
| `__init__.py` | ESPHome config schema + `to_code()` codegen |
| `bs_pool_X.h` | C++ class: BSPoolListener + Component, SUB_* macros |
| `bs_pool_X.cpp` | `codes_to_poll()` + `handle_message()` with switch on function_code |

Switch and select additionally have child classes (`UserSettingsSwitch`, `ControlModeSelect`) using `Parented<BSPool>` for write-back to the UART bus.

## Protocol

- **Baud**: 19200
- **Request**: `{'?', function_code, '\4'}` (3 bytes)
- **Response**: `{function_code, data_b2, data_b3}` (3 bytes)
- **DataPacket**: union of `{function_code, data_b2, data_b3}` and raw `uint8_t data[3]`

### Function Codes

| Code | Name | Usage |
|------|------|-------|
| `'r'` | USER | User settings bitfield |
| `'m'` | CONTROL_MODE | Manual/Auto/Semi-auto |
| `'A'` | ALARMS | Alarm bitmask |
| `'w'` | WARNINGS | Warning bitmask |
| `'s'` | STATUS | Status bitmask |
| `'c'`/`'C'`/`'V'`/`'p'`/`'N'`/`'o'`/`'W'` | Measurements | Various sensor values |
| `'H'` | TIME | Clock |
| `'y'` | VERSION | Firmware version |

## Conventions

- **Language**: C++ (Arduino/ESP8266), Python (ESPHome codegen)
- **Commits**: Conventional Commits in English (`feat:`, `fix:`, `chore:`)
- **Branch**: `main`
- **No CI/CD, no linter, no formatter** — early-stage project
- **Hardware target**: ESP8266 NodeMCU + RS232-TTL converter + DC-DC 5V step-down
- **Test**: Build-only test via `esphome compile` (no unit tests)

## Key Gotchas

- **Sensor scaling**: cell_current/voltage → `/10.0f`, pH/salt → `/100.0f`, radox → raw mV
- **Invalid values**: pH and salt report `0xFFFF` when unavailable → publish `NAN`
- **Temperature sign**: `data_b3` flag controls sign of temperature
- **Inverted switch bits**: `is_outdoor` (bit0), `ph_alarm` (bit4), `ph_control` (bit6) use inverted logic
- **TODO**: `bs_pool_switch.cpp:34` — unverified bit for `cover_switch_off`
