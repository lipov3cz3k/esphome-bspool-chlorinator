# bs_pool Component

ESPHome external component implementing the BSPool chlorinator UART protocol.

## Module Layout

```
__init__.py + bs_pool.{h,cpp}   # Hub: BSPool, BSPoolListener, DataPacket, FunctionCode
sensor/                          # 7 sensors (cell_current, cell_intensity, cell_voltage, ph, salt, radox, temp)
binary_sensor/                   # 21 binary sensors (7 alarms, 6 warnings, 8 statuses)
switch/                          # 8 user settings switches (bitfield on FunctionCode::USER)
select/                          # 1 select: control_mode (FunctionCode::CONTROL_MODE)
text_sensor/                     # 2 text sensors: time (FunctionCode::TIME), version (FunctionCode::VERSION)
```

## Adding a New Entity

1. **Create sub-directory** with `__init__.py`, `bs_pool_X.h`, `bs_pool_X.cpp`
2. **Python schema** (`__init__.py`):
   - Define `CONFIG_SCHEMA` with `cv.Optional` fields keyed by entity name
   - `async def to_code(config)` → `cg.new_Pvariable()`, set parent, `register_listener()`
3. **C++ header** (`bs_pool_X.h`):
   - Inherit `BSPoolListener` + `Component`
   - Use `SUB_SENSOR` / `SUB_BINARY_SENSOR` / `SUB_SWITCH` / `SUB_TEXT_SENSOR` macros for each entity
4. **C++ source** (`bs_pool_X.cpp`):
   - `codes_to_poll()` → return vector of `FunctionCode`s this component needs
   - `handle_message(DataPacket &packet)` → switch on `packet.function_code`, decode, `publish_state()`
5. **For writable entities** (switch/select): add write helper methods in the component and send UART commands with `parent_->write_array({code, b2, b3})` + `flush()`

### Checklist

- [ ] Add `CONF_*` constant in `__init__.py` if not in `esphome.const`
- [ ] Register as listener in `to_code()`: `cg.add(hub.register_listener(var))`
- [ ] Add new entries to `tests/components/bs_pool/common.yaml`
- [ ] Verify with `esphome compile tests/components/bs_pool/test.esp8266-ard.yaml`

## Protocol Details

### Data Decoding Patterns

```cpp
// Simple uint16 from B2/B3 (B2 = low byte, B3 = high byte)
uint16_t raw = (packet.data_b3 << 8) | packet.data_b2;

// Scaled float (cell_current, cell_voltage)
float value = raw / 10.0f;

// Scaled float (pH, salt_concentration)
float value = raw / 100.0f;

// Invalid marker (pH, salt only)
if (raw == 0xFFFF) { publish_state(NAN); return; }

// Signed temperature (data_b3 bit controls sign)
float temp = packet.data_b2;
if (packet.data_b3) temp = -temp;

// Bitmask (alarms/status in B2; warnings split across B2/B3)
bool flag = packet.data_b2 & (1 << BIT_POSITION);

// User settings bitfield (switch)
bool setting = packet.data_b2 & (1 << BIT_POSITION);
// INVERTED bits: is_outdoor(bit0), ph_alarm(bit4), ph_control(bit6)
```

### Writing Data (switch/select)

```cpp
// BSPoolSwitch::send_user_state()
// Pack all 8 user setting bits into B2 and send full USER bitfield
parent_->write_array({FunctionCode::USER, packed_user_settings_b2, '\4'});
parent_->flush();

// BSPoolSelect::send_command(FunctionCode::CONTROL_MODE, mode_value)
parent_->write_array({FunctionCode::CONTROL_MODE, mode_value, '\4'});
parent_->flush();
```

## Bitmask Maps

### Alarms (FunctionCode::ALARMS = `'A'`, data_b2)

| Bit | Name | Description |
|-----|------|-------------|
| 0 | `alarm_redox_saturated` | ORP probe saturated |
| 1 | `alarm_overtemperature` | Overtemperature inside chlorinator |
| 2 | `alarm_open_circuit` | Open circuit |
| 3 | `alarm_water_flow` | Lack of water flow |
| 4 | `alarm_short_circuit` | Short circuit |
| 5 | `alarm_unit_stopped` | P954 unit stopped |
| 6 | `alarm_communication_error` | Communication error to P954 |

### Warnings (FunctionCode::WARNINGS = `'w'`)

| Bit | Name | Description |
|-----|------|-------------|
| B2.2 | `warning_low_salt` | Too few salt |
| B2.3 | `warning_high_salt` | Too much salt |
| B3.0 | `warning_ph_pump_overrun` | pH problem — pump has run too long |
| B3.1 | `warning_acid` | Acid warning |
| B3.2 | `warning_clock_set` | Clock has been set |
| B3.3 | `warning_probe_disconnected` | ORP/chlorine probe disconnected |

### Status (FunctionCode::STATUS = `'s'`, data_b2)

| Bit | Name |
|-----|------|
| 0 | `status_output_voltage_polarity` |
| 1 | `status_filter_pump_running` |
| 2 | `status_chlorination_process` |
| 3 | `status_stopped_by_remote` |
| 4 | `status_orp_sensor_stable` |
| 5 | `status_salt_sensor_connected` |
| 6 | `status_electrode_cleaning` |
| 7 | `status_super_chlorination` |

### User Settings (FunctionCode::USER = `'r'`, data_b2)

| Bit | Name | Inverted? |
|-----|------|-----------|
| 0 | `is_outdoor` | ✓ |
| 1 | `cover_switch_off` | ✗ (TODO: unverified) |
| 2 | `flow_switch_installed` | ✗ |
| 3 | `orp_displayed` | ✗ |
| 4 | `ph_alarm` | ✓ |
| 5 | `ph_corrector_alkaline` | ✗ |
| 6 | `ph_control` | ✓ |
| 7 | `cover_installed` | ✗ |
