# BSPool Chlorinator Serial Protocol

> For software version 70 or higher.

## Communication Parameters

| Parameter      | Value  |
|----------------|--------|
| Baud rate      | 19200  |
| Data bits      | 8      |
| Parity         | None   |
| Stop bits      | 1      |
| Flow control   | None   |
| Frame size     | 3 bytes (always) |
| Max inter-byte delay | 10 ms (incomplete frames are discarded) |

## Master / Slave Model

- The **remote controller is the master** — the chlorinator is a slave and never sends data unsolicited.

## Frame Format

### Request (master → chlorinator)

```
Byte 0: '?' (0x3F)
Byte 1: function_code (B1)
Byte 2: EOT (0x04)
```

### Response (chlorinator → master)

```
Byte 0: function_code (B1) — echo of requested code
Byte 1: data_b2 (B2)
Byte 2: data_b3 (B3)
```

### Write Command (master → chlorinator)

```
Byte 0: function_code (B1) — parameter to change
Byte 1: data low byte (B2)
Byte 2: data high byte (B3)
```

Response: same format as read response. Out-of-range values are rejected.

### Error Response

```
Byte 0: 'E' (0x45)
Byte 1: 'R' (0x52)
Byte 2: error code (n)
```

| Error code | Meaning |
|------------|---------|
| 1 | Unknown command (first byte) |
| 2 | Unknown parameter (2nd byte after `'?'`) |
| 3 | Timeout error |

---

## P906 Function Codes

### Device Info

| B1 | Description | Unit | Writable | B2 | B3 |
|----|-------------|------|----------|----|----|
| `'Z'` (0x5A) | Chlorinator size/type | — | No | 0=P927 10g/h<br>1=P927 15g/h<br>2=P950 20g/h<br>3=P950 25g/h<br>4=P953 35g/h<br>5=P956 50g/h<br>6=P956 70g/h<br>7=P957 100g/h<br>8=P957 150g/h<br>9=P954 200g/h<br>10=P910 35g/h<br>11=P910 50g/h<br>12=P910 70g/h<br>13=P910 100g/h<br>14=P910 200g/h | EOT (0x04) |
| `'y'` (0x79) | Software version | — | No | Version integer part | Version decimal part |

### Settings

| B1 | Description | Unit | Writable | B2 | B3 |
|----|-------------|------|----------|----|----|
| `'L'` (0x4C) | Language | — | Yes | 0=English<br>1=Spanish<br>2=Catalan<br>3=French<br>4=Italian<br>5=Dutch<br>6=Portuguese<br>7=Turkish<br>8=Hebrew<br>9=German<br>10=Czech | EOT (0x04) |
| `'v'` (0x76) | Swimming pool volume | 1 m³ | Yes | Volume low byte | Volume high byte |
| `'m'` (0x6D) | Control mode | — | Yes | 0=Manual<br>1=Automatic<br>2=Semi-automatic | EOT (0x04) |
| `'b'` (0x62) | Cleaning cycle | 10 min | Yes | Delay (minutes/10) | EOT (0x04) |
| `'T'` (0x54) | Select power 0–100% | % | Yes | Power | EOT (0x04) |
| `'S'` (0x53) | Stop/Start (also stops pH pump, clears alarms) | — | Yes | 0=Start, 1=Stop | EOT (0x04) |
| `'r'` (0x72) | User settings bitfield | — | Yes | Bit 0: 0=Outdoors / 1=Indoors<br>Bit 1: 0=Cover switch ON when cover ON / 1=OFF<br>Bit 2: 0=Flow switch not installed / 1=Installed<br>Bit 3: 0=ORP not displayed / 1=Displayed<br>Bit 4: 0=pH alarm active / 1=Off<br>Bit 5: 0=pH corrector acid / 1=Alkaline<br>Bit 6: 0=pH control on / 1=Off<br>Bit 7: 0=Cover not installed / 1=Installed | EOT (0x04) |
| `'P'` (0x50) | pH target (×100, e.g. 7.10 → 710) | — | Yes | pH low byte | pH high byte |

### Alarms, Warnings & Status

| B1 | Description | Unit | Writable | B2 | B3 |
|----|-------------|------|----------|----|----|
| `'A'` (0x41) | Alarm bitmask | — | No | Bit 0: ORP probe saturated<br>Bit 1: Overtemperature<br>Bit 2: Open circuit<br>Bit 3: Lack of water flow<br>Bit 4: Short circuit<br>Bit 5: P954 unit stopped<br>Bit 6: Communication error to P954 | EOT (0x04) |
| `'w'` (0x77) | Warning flags | — | No | Bit 2: Too few salt<br>Bit 3: Too much salt | Bit 0: pH pump overrun<br>Bit 1: Acid warning<br>Bit 2: Clock has been set<br>Bit 3: ORP/chlorine probe disconnected |
| `'s'` (0x73) | Status bitmask | — | No | Bit 0: Polarity of output voltage<br>Bit 1: Filter pump running<br>Bit 2: Chlorination in process<br>Bit 3: Stopped by remote controller<br>Bit 4: ORP sensor reading is stable<br>Bit 5: Salt sensor connected<br>Bit 6: Electrode cleaning in process<br>Bit 7: Super-chlorination in process | EOT (0x04) |

### Measurements

| B1 | Description | Unit | Scaling | B2 | B3 |
|----|-------------|------|---------|----|----|
| `'c'` (0x63) | Cell current | 100 mA | ÷10 → Amps | I low byte | I high byte |
| `'C'` (0x43) | Cell intensity (% of max) | % | Raw | Percent | EOT (0x04) |
| `'V'` (0x56) | Cell voltage | 100 mV | ÷10 → Volts | V low byte | V high byte |
| `'p'` (0x70) | pH measurement | 0.01 pH | ÷100 → pH | pH low byte | pH high byte |
| `'N'` (0x4E) | Salt concentration | g/l × 100 | ÷100 → g/l | Salt low byte | Salt high byte |
| `'o'` (0x6F) | Current ORP value | mV | Raw | ORP low byte | ORP high byte |
| `'W'` (0x57) | Water temperature | 1°C | Raw (signed) | Temperature | Sign (0=+, 1=−) |

**Special values:**
- pH (`'p'`): `B2=0xFF, B3=0xFF` → sensor not connected
- Salt (`'N'`): `B2=0xFF, B3=0xFF` → sensor not connected
- Temperature (`'W'`): `B2=0xFF, B3=0x01` → sensor not connected
- ORP (`'o'`): accepts values up to `0x7FF`

**ORP / Free Chlorine dual reading:**
- `'O'` (0x4F) — **target** ORP (writable): B2=ORP low, B3=ORP high. For free chlorine: B2=ppm×100, B3=`0b10000000`
- `'o'` (0x6F) — **current** ORP (read-only): B2=ORP low, B3=ORP high. For free chlorine: B3 highest bit set (`0b1xxxxxxx`), remaining = ppm high

### Time & Hours of Operation

| B1 | Description | Writable | B2 | B3 |
|----|-------------|----------|----|----|
| `'H'` (0x48) | Current time | Yes | Minutes (0–59) | Hours (0–23) |
| `'F'` (0x46) | Hours of operation (low) | No | Minutes (0–59) | Hours low byte |
| `'G'` (0x47) | Hours of operation (high) | No | Hours middle byte | Hours high byte |

### Relay Control

| B1 | Description | Writable | B2 | B3 |
|----|-------------|----------|----|----|
| `'R'` (0x52) | Relay status | Yes | Read: 0=off, 1=on<br>Write: 0=set OFF, 1=set ON, 2=timed ON, 3=1 cycle/24h, 4=2 cycles/24h | EOT (0x04) |
| `'D'` (0x44) | Relay delay (when status=1) | Yes | Minutes | EOT (0x04) |
| 201 | Program 1 start time | Yes | Minutes | Hours |
| 202 | Program 1 stop time | Yes | Minutes | Hours |
| 203 | Program 2 start time | Yes | Minutes | Hours |
| 204 | Program 2 stop time | Yes | Minutes | Hours |

### P954 Multi-Unit Readings (numeric B1)

| B1 | Description | Unit |
|----|-------------|------|
| 131 | Cell intensity %, P954 unit 1 | % |
| 132 | Cell intensity %, P954 unit 2 | % |
| 133 | Cell intensity %, P954 unit 3 | % |
| 134 | Cell intensity %, P954 unit 4 | % |
| 141 | Cell voltage ×10, P954 unit 1 | 100 mV |
| 142 | Cell voltage ×10, P954 unit 2 | 100 mV |
| 143 | Cell voltage ×10, P954 unit 3 | 100 mV |
| 144 | Cell voltage ×10, P954 unit 4 | 100 mV |
| 211 | Hours of operation, P954 unit 1 | Hours |
| 212 | Hours of operation, P954 unit 2 | Hours |
| 213 | Hours of operation, P954 unit 3 | Hours |
| 214 | Hours of operation, P954 unit 4 | Hours |

---

## P913 Function Codes

The P913 chlorinator uses a subset/variant of the P906 protocol.

### P913-specific Differences

| B1 | Description | B2 | B3 |
|----|-------------|----|----|
| `'L'` | Language (same as P906 but without Czech) | 0–9 | EOT (0x04) |
| `'m'` | Control mode | 0=Manual<br>1=Automatic (no Semi-auto) | EOT (0x04) |
| `'T'` | Select power 0–100% | % Power | EOT (0x04) |
| `'C'` | Selected power 0–100% (read-only) | % Power | EOT (0x04) |

### P913 Alarms, Warnings & User Settings

| B1 | Description | Unit | Writable | B2 | B3 |
|----|-------------|------|----------|----|----|
| `'A'` | Alarm bitmask | — | No | Bit 0: ORP problem — pump overrun<br>Bit 3: Lack of water flow | EOT (0x04) |
| `'w'` | Warning flags | — | No | Bit 0: pH problem — pump overrun | EOT (0x04) |
| `'r'` | User settings bitfield | — | Yes | Bit 2: 0=Flow switch disabled / 1=Enabled<br>Bit 3: 0=ORP not displayed / 1=Displayed<br>Bit 5: 0=pH corrector acid / 1=Alkaline<br>Bit 6: 0=pH control enabled / 1=Disabled<br>Bit 7: 0=Chlorine control enabled / 1=Disabled | EOT (0x04) |

### P913 Shared Commands

Same as P906: `'O'`, `'o'`, `'P'`, `'p'`, `'F'`, `'G'`, `'y'`


