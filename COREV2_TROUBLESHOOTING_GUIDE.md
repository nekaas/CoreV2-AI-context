# CoreV2 Summer Camp — Complete Troubleshooting Guide

> **What could possibly go wrong?** — Everything. Here's how to fix it.  
> Target: ESP32-S3 N16R8 (CoreV2), ESP32-C3 Supermini (remote nodes), Arduino IDE 2.x  
> 4 Projects: Game Boy · Smart Plant Pot · Smart Safe · Security System  
> **🇱🇻 Valoda:** Studenti runā latviski. Tulko kļūdu ziņojumus latviski. Izmanto latviešu terminu glosāriju no `COREV2_PCB_FULL_CONTEXT.md`.

---

## ⚡ Severity & Fix Time Legend

| Icon | Severity | Typical Fix Time | Meaning |
|------|----------|-----------------|---------|
| 💀 | CRITICAL | 1 min | Board crash, data loss, permanent damage risk |
| 🚨 | HIGH | 5-15 min | Feature completely broken, student blocked |
| ⚠️ | MEDIUM | 2-10 min | Works but wrong/ugly/slow |
| ℹ️ | LOW | 1-2 min | Cosmetic, optimization, best practice |

---

## 🇱🇻 Latvian Error Translation Index

> When a student describes a problem in Latvian, use this index to find the right section.

| Student Says (Latvian) | Go To Section |
|------------------------|---------------|
| "Ekrāns ir melns / nestrādā / nerāda" | [A4 I²C Bus](#a4-i²c-bus-problems-oled--pcf8574) or [B1 OLED](#b1-oled-display-rendering) |
| "Pogas nestrādā / nereaģē" | [A4.4 PCF8574](#a4-i²c-bus-problems-oled--pcf8574) or [B2 D-Pad](#b2-d-pad--pcf8574-button-input) |
| "Nevaru augšupielādēt kodu" | [A1 Upload Failures](#a1-arduino-ide--upload-failures) |
| "Plate pārstartējas" / "Restartējas" | [A7 Boot Loops](#a7-boot-loops-crashes--watchdogs) |
| "Plate ir izslēgusies / beigta" | [A5 Power](#a5-power-battery--polyfuse) |
| "Servo motors tikai raustās / trīc" | [D1 Servo](#d1-continuous-rotation-servo) or [D2 Power](#d2-servo-power-supply) |
| "Servo negriežas vispār" | [D1 Servo](#d1-continuous-rotation-servo) |
| "Zummers klusē / nav skaņas" | [B4 Buzzer](#b4-buzzer-sound-effects) |
| "Spēle ir lēna / bremzē" | [B6 Game Logic](#b6-game-logic--state-machines) |
| "Attēls uz ekrāna ir nobīdīts / dīvains" | [B1.2 OLED Shift](#b1-oled-display-rendering) |
| "SD karte nestrādā / nevar nolasīt" | [B5 SD Card](#b5-microsd-card-saveload) |
| "Mitruma sensors rāda nepareizi" | [C1 Moisture](#c1-capacitive-soil-moisture-sensor) or [F1 Deep Dive](#f1-capacitive-moisture-sensor-deep-dive) |
| "Temperatūra ir nepareiza / -273°C" | [C2 Thermistor](#c2-ntc-thermistor-temperature) or [F2 Deep Dive](#f2-ntc-thermistor-deep-dive) |
| "WiFi nevar pieslēgties" | [C7 WiFi](#c7-wifi-iot-dashboard-optional) or [A8 Libraries](#a8-library--dependency-hell) |
| "ESP-NOW ziņas nesaņem" | [E3 ESP-NOW](#e3-esp-now-wireless-mesh) |
| "PIR sensors visu laiku rāda kustību" | [E1 PIR](#e1-pir-motion-sensor-hc-sr501--am312) or [F3 Deep Dive](#f3-pir-motion-sensor-deep-dive) |
| "IR stars nestrādā" | [E2 IR](#e2-ir-receiver--transmitter-beam) or [F4 Deep Dive](#f4-ir-communication-deep-dive) |
| "C3 Supermini neprogrammējas" | [E4 C3](#e4-esp32-c3-supermini-remote-nodes) |
| "Pazuda saglabātie dati" | [D5 LittleFS](#d5-littlefs-config-storage) |
| "Kompilācijas kļūda" / "Nekomplejējas" | [A1.3 Libraries](#a1-arduino-ide--upload-failures) or [A8 Dependency](#a8-library--dependency-hell) |
| "Viss bija labi, tagad nestrādā" | [A5 Power](#a5-power-battery--polyfuse) — polyfuse first! |
| "Kaut kas smird / kūp" | 💀 IMMEDIATELY unplug power! Short circuit somewhere. Visually inspect. |
| "Džoistiks pats kustas" / "Tēls iet pats" | [B3 Joystick](#b3-analog-joystick) — deadzone calibration |
| "PIN kods nestrādā" / "Nevar atslēgt" | [D3 PIN Entry](#d3-pin-entry-ui--debounce) or [D4 Auth](#d4-authentication--security) |
| "Nepareizs pogu izkārtojums" / "Augšu/leju samainītas" | [B2 D-Pad](#b2-d-pad--pcf8574-button-input) — PCF8574 bit mapping |

---

## Table of Contents

**PART A — Universal (All Projects)**
- [A1. Arduino IDE & Upload Failures](#a1-arduino-ide--upload-failures)
- [A2. Board Selection & Configuration](#a2-board-selection--configuration)
- [A3. GPIO & Pin Catastrophes](#a3-gpio--pin-catastrophes)
- [A4. I²C Bus Problems (OLED + PCF8574)](#a4-i²c-bus-problems-oled--pcf8574)
- [A5. Power, Battery & Polyfuse](#a5-power-battery--polyfuse)
- [A6. Serial Monitor Issues](#a6-serial-monitor-issues)
- [A7. Boot Loops, Crashes & Watchdogs](#a7-boot-loops-crashes--watchdogs)
- [A8. Library & Dependency Hell](#a8-library--dependency-hell)
- [A9. Memory Exhaustion (Heap & Stack)](#a9-memory-exhaustion-heap--stack)
- [A10. Compiler & Language Gotchas](#a10-compiler--language-gotchas)

**PART B — Project: Game Boy 🎮**
- [B1. OLED Display Rendering](#b1-oled-display-rendering)
- [B2. D-Pad & PCF8574 Button Input](#b2-d-pad--pcf8574-button-input)
- [B3. Analog Joystick](#b3-analog-joystick)
- [B4. Buzzer Sound Effects](#b4-buzzer-sound-effects)
- [B5. microSD Card (Save/Load)](#b5-microsd-card-saveload)
- [B6. Game Logic & State Machines](#b6-game-logic--state-machines)
- [B7. Animation & Sprites on OLED](#b7-animation--sprites-on-oled)
- [B8. I²S DAC Audio (Future)](#b8-i²s-dac-audio-future)

**PART C — Project: Smart Plant Pot 🌱**
- [C1. Capacitive Soil Moisture Sensor](#c1-capacitive-soil-moisture-sensor)
- [C2. NTC Thermistor Temperature](#c2-ntc-thermistor-temperature)
- [C3. ADC Reading & Noise](#c3-adc-reading--noise)
- [C4. Threshold Alerts & Hysteresis](#c4-threshold-alerts--hysteresis)
- [C5. PCF8574 LED Moisture Indicator](#c5-pcf8574-led-moisture-indicator)
- [C6. OLED Plant Dashboard](#c6-oled-plant-dashboard)
- [C7. WiFi IoT Dashboard (Optional)](#c7-wifi-iot-dashboard-optional)

**PART D — Project: Smart Safe 🔐**
- [D1. Continuous Rotation Servo](#d1-continuous-rotation-servo)
- [D2. Servo Power Supply](#d2-servo-power-supply)
- [D3. PIN Entry UI & Debounce](#d3-pin-entry-ui--debounce)
- [D4. Authentication & Security](#d4-authentication--security)
- [D5. LittleFS Config Storage](#d5-littlefs-config-storage)
- [D6. State Machine Logic](#d6-state-machine-logic)
- [D7. Web Unlock Interface (Optional)](#d7-web-unlock-interface-optional)

**PART E — Project: Security System 🚨**
- [E1. PIR Motion Sensor (HC-SR501 / AM312)](#e1-pir-motion-sensor-hc-sr501--am312)
- [E2. IR Receiver & Transmitter Beam](#e2-ir-receiver--transmitter-beam)
- [E3. ESP-NOW Wireless Mesh](#e3-esp-now-wireless-mesh)
- [E4. ESP32-C3 Supermini Remote Nodes](#e4-esp32-c3-supermini-remote-nodes)
- [E5. Alarm Siren Logic](#e5-alarm-siren-logic)
- [E6. SD Card Event Logging](#e6-sd-card-event-logging)
- [E7. Multi-Zone Coordination](#e7-multi-zone-coordination)

**PART F — Sensor-Specific Deep Dives**
- [F1. Capacitive Moisture Sensor Deep Dive](#f1-capacitive-moisture-sensor-deep-dive)
- [F2. NTC Thermistor Deep Dive](#f2-ntc-thermistor-deep-dive)
- [F3. PIR Motion Sensor Deep Dive](#f3-pir-motion-sensor-deep-dive)
- [F4. IR Communication Deep Dive](#f4-ir-communication-deep-dive)
- [F5. Analog Joystick Deep Dive](#f5-analog-joystick-deep-dive)

---

## A1. Arduino IDE & Upload Failures

### A1.1 — "Port not showing up" / "No board connected"

```
Symptom: Tools → Port is grayed out. No COM port appears.
```

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| USB cable is power-only (no data lines) | Try a different cable. Charge-only cables are VERY common | Use a cable known to work for data (test with phone file transfer) |
| USB-C connector dirty or loose | Wiggle the connector — does it connect intermittently? | Clean with compressed air. Try a different USB-C cable |
| USB CDC not enabled in firmware | Board was flashed with USB CDC disabled | Re-flash with USB CDC On Boot: Enabled. If you can't flash, hold BOOT button while plugging in, then flash |
| Driver not installed (Windows) | Device Manager shows "Unknown device" | Install CP210x or CH340 drivers (ESP32-S3 uses built-in USB-JTAG, no external chip — should be automatic on Win 10+) |
| macOS blocking accessory | System Preferences → Security might have blocked it | Allow in Security & Privacy settings |
| Board in boot loop (constant reset) | Serial monitor shows repeating boot messages | See Section A7 |
| Polyfuse tripped | Board is completely dead, no LED | Unplug USB, wait 30 seconds, plug back in |

### A1.2 — "Upload fails with timeout / sync error"

```
Symptom: "Connecting... _______......_____" then "Failed to connect" or timeout.
```

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Wrong board selected | Tools → Board shows something other than "ESP32S3 Dev Module" | Select: **ESP32S3 Dev Module** |
| Wrong upload speed | Upload speed too high for cable/port | Try 115200 or 460800 instead of 921600 |
| Board not in download mode | ESP32-S3 needs to enter download mode | Hold **BOOT** button, press **RESET**, release **RESET**, then release **BOOT** |
| Another program using the port | Serial Monitor still open from last session | Close Serial Monitor before uploading |
| Flash size mismatch | Partition scheme expects different flash size | Set Flash Size to **16MB (128Mb)** |
| USB cable issue | Intermittent connection during upload | Use a shorter, higher-quality USB cable |

### A1.3 — "Compilation error: 'Adafruit_SSD1306.h' not found"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Library not installed | Sketch → Include Library → Manage Libraries | Install: **Adafruit SSD1306** by Adafruit |
| Wrong library version | Version incompatibility | Install the latest version. If that breaks things, downgrade to 2.5.x |
| Missing dependency | Adafruit SSD1306 needs Adafruit GFX and Adafruit BusIO | Install **Adafruit GFX Library** and **Adafruit BusIO** |
| Multiple library copies | Two versions installed in different locations | Delete duplicates from `~/Documents/Arduino/libraries/` |

### A1.4 — "Compilation error: 'Wire.h' / 'SPI.h' not found"

These are **built-in** to the ESP32 Arduino core. If they're missing, the ESP32 board package isn't installed correctly.

**Fix:**
1. File → Preferences → Additional Boards Manager URLs
2. Add: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Tools → Board → Boards Manager → Search "esp32" → Install **esp32 by Espressif Systems** (version 2.0.x or 3.x)

### A1.5 — "Sketch too big" / "Section `.text` will not fit"

```
Symptom: "Sketch uses 3400000 bytes (212%) of flash"
```

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Partition scheme too small | Default 4MB scheme can't fit large sketches | Change Tools → Partition Scheme to **16M Flash (3MB APP/9MB FAT)** or **Huge APP (3MB)** |
| Too many PROGMEM arrays | Large animation/sprite data exceeding partition | Split into multiple files. Use SD card for bulk assets. Remove unused animations |
| Debug symbols bloating binary | Compiling with debug flags | Set Tools → Core Debug Level to **None** |
| PSRAM not used for heap | All data going to internal DRAM | Enable PSRAM: Tools → PSRAM → **OPI PSRAM** |

---

## A2. Board Selection & Configuration

### The Correct Settings (Tattoo This on Your Forearm)

```
Arduino IDE → Tools:
  Board:               "ESP32S3 Dev Module"
  USB CDC On Boot:     "Enabled"
  CPU Frequency:       "240MHz (WiFi)"
  Core Debug Level:    "None"
  USB DFU On Boot:     "Disabled"
  Erase All Flash:     "Disabled"
  Events Run On:       "Core 1"
  Flash Mode:          "QIO 80MHz"
  Flash Size:          "16MB (128Mb)"
  JTAG Adapter:        "Disabled"
  Partition Scheme:    "16M Flash (3MB APP/9MB FAT)"  ← for Doom/SD projects
                       "Default 4MB with spiffs"       ← for LittleFS projects
  PSRAM:               "OPI PSRAM"
  Upload Mode:         "UART0 / Hardware CDC"
  Upload Speed:        "921600"
  USB Mode:            "Hardware CDC and JTAG"
```

### A2.1 — "I selected the wrong board, now the ESP32 won't respond"

Incorrect board settings can flash firmware with wrong pin mappings, bricking the board until re-flashed with correct settings.

**Recovery procedure:**
1. Hold **BOOT** button on CoreV2 PCB
2. While holding BOOT, press and release **RESET**
3. Release BOOT (board is now in download mode)
4. In Arduino IDE: Tools → set ALL settings correctly (see table above)
5. Sketch → Upload
6. After upload completes, press RESET

### A2.2 — "PSRAM not working" / "ESP.getFreePsram() returns 0"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| PSRAM not enabled in Tools | `ESP.getFreePsram()` returns 0 | Tools → PSRAM → **OPI PSRAM** |
| Wrong PSRAM mode | Selected "Quad PSRAM" instead of "OPI PSRAM" | N16R8 uses Octal PSRAM — must be OPI mode |
| PSRAM pins in use as GPIO | Student used GPIO33 or GPIO35 as an output | ⛔ NEVER use GPIO22-37 — they ARE the PSRAM bus! |
| Faulty hardware | PSRAM chip damaged | Production test `testMCU()` would have caught this |

---

## A3. GPIO & Pin Catastrophes

### A3.1 — THE #1 MISTAKE: Using GPIO22–37

```
⛔ GPIO22, GPIO23, GPIO24, GPIO25, GPIO26, GPIO27,
   GPIO28, GPIO29, GPIO30, GPIO31, GPIO32, GPIO33,
   GPIO34, GPIO35, GPIO36, GPIO37

THESE ARE THE OCTAL PSRAM BUS.
If you pinMode() ANY of these, the board WILL:
  - Crash immediately
  - Corrupt memory
  - Reboot randomly
  - Produce garbage data
  - Appear to work for 10 seconds then die
```

**How to catch this:**
```cpp
void setup() {
  // Defensive check — add to every project!
  for (uint8_t p = 22; p <= 37; p++) {
    if (digitalPinIsValid(p)) {
      Serial.printf("⛔ WARNING: GPIO%d is in use! This is a PSRAM pin!\n", p);
    }
  }
}
```

### A3.2 — "Pin doesn't do what I expect"

| Symptom | Common Cause | Fix |
|---------|-------------|-----|
| `digitalWrite(pin, HIGH)` does nothing | Pin not set to OUTPUT with `pinMode(pin, OUTPUT)` | Always call `pinMode()` before `digitalWrite()` |
| `digitalRead(pin)` always returns 0 or 1 | Pin is floating (no pull-up/down) | Use `pinMode(pin, INPUT_PULLUP)` or add external resistor |
| `analogRead(pin)` returns weird values | Wrong attenuation | ADC defaults to 0-1.1V range! Use `analogSetAttenuation(ADC_11db)` for 0-3.3V |
| Pin works sometimes, not others | Shared pin conflict (e.g., GPIO4 used for both joystick AND encoder) | Check COREV2_QUICKREF.h for conflicts |
| `digitalWrite()` on GPIO34-37 does nothing | These are input-only pins | GPIO34-39 are input-only on ESP32. Use GPIO38 or others for output |

### A3.3 — GPIO Current Limits

```
ESP32-S3 GPIO maximum current:  ~40 mA per pin
Recommended continuous current: ~20 mA per pin
Total GPIO current (all pins):  ~200 mA

Exceeding these WILL damage the ESP32-S3 permanently.
The buzzer uses a transistor (Q2) specifically because
the GPIO can't drive it directly!
```

---

## A4. I²C Bus Problems (OLED + PCF8574)

### A4.1 — I²C Scanner Shows Nothing

```cpp
// ═══ I²C Diagnostic Scanner ═══
// Run this FIRST when OLED or buttons don't work
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);  // ⚠️ MUST be GPIO8=SDA, GPIO9=SCL
  Serial.println("\n=== I2C Scanner ===");

  byte count = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  ✅ Device found at 0x%02X", addr);
      if (addr == 0x3C) Serial.print(" ← OLED (SSD1306)");
      if (addr == 0x3D) Serial.print(" ← OLED (alt address)");
      if (addr == 0x20) Serial.print(" ← PCF8574 (buttons+LEDs)");
      Serial.println();
      count++;
    }
  }
  if (count == 0) {
    Serial.println("  ❌ NO DEVICES FOUND!");
    Serial.println("  Check: Wire.begin(8,9)? Pull-up resistors (R1,R2=4.7kΩ)? Power?");
  }
  Serial.printf("  Found %d device(s). Expected: 2 (0x20 + 0x3C)\n", count);
}

void loop() {}
```

### A4.2 — "OLED found at 0x3C but screen is blank"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| `display.display()` never called | Check code — drawing commands need `display.display()` to push to screen | Add `display.display()` after drawing |
| OLED VCC not connected | Check board — is OLED module properly soldered? | Reflow solder joints on OLED header |
| OLED contrast/brightness issue | Screen is on but too dim to see | SSD1306 doesn't have contrast control (it's always full brightness). If dim, the display may be damaged |
| Wrong `begin()` call | Used `SSD1306_SWITCHCAPVCC` on external VCC display | Try `display.begin(SSD1306_EXTERNALVCC, 0x3C)` |
| SH1106 vs SSD1306 mismatch | Screen is powered but image is shifted 2px right | Change `BoardConfig::kOledPanelType` to `OledPanelType::SH1106` (or vice versa) |

### A4.3 — "I²C bus hangs / freezes after running for a while"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| I²C clock stretching | Slave device holds clock line low too long | Increase I²C timeout: `Wire.setTimeOut(100);` (default is 50ms) |
| Electrical noise on SDA/SCL | Long wires picking up interference | Keep I²C wires short (< 20cm). Check pull-ups are 4.7kΩ |
| Bus contention | Two devices trying to talk simultaneously | I²C is multi-master capable but Arduino Wire library doesn't support it. Only call Wire functions from one task/core |
| Power glitch | OLED/PCF8574 briefly losing power | Add 100nF bypass capacitor near each I²C device's VCC pin |

### A4.4 — "PCF8574 found but buttons don't work"

```cpp
// ═══ PCF8574 Button Diagnostic ═══
void loop() {
  Wire.beginTransmission(0x20);
  Wire.write(0xFF);  // Set all pins as inputs
  Wire.endTransmission();
  Wire.requestFrom(0x20, 1);

  if (Wire.available()) {
    uint8_t raw = Wire.read();
    uint8_t pressed = ~raw;  // Invert — active LOW

    Serial.print("Raw=0x"); Serial.print(raw, HEX);
    Serial.print(" Pressed=");
    if (pressed & 0x01) Serial.print("P0(UP) ");
    if (pressed & 0x02) Serial.print("P1(DOWN) ");
    if (pressed & 0x04) Serial.print("P2(LEFT) ");
    if (pressed & 0x08) Serial.print("P3(RIGHT) ");
    if (pressed & 0x80) Serial.print("P7(CENTER) ");
    if (pressed == 0)   Serial.print("(none)");
    Serial.println();
  } else {
    Serial.println("❌ Wire.requestFrom returned no data!");
  }
  delay(200);
}
```

**Common mistakes:**
- Forgetting to invert (`~raw`) — buttons are **active LOW**
- Reading the wrong bit — P7 is bit 7 (0x80), not bit 4
- Not writing 0xFF first — pins default to outputs, must set as inputs
- Interpreting `raw == 0` as "no button" — raw=0x00 means ALL buttons pressed (impossible unless all 5 are held down)

---

## A5. Power, Battery & Polyfuse

### A5.1 — "Board is completely dead, no LEDs, nothing"

| Step | Action |
|------|--------|
| 1 | Unplug USB-C cable |
| 2 | Wait **30 seconds** (polyfuse needs time to cool down and reset) |
| 3 | Plug USB-C back in |
| 4 | If still dead, try a different USB cable AND a different USB port/charger |
| 5 | If STILL dead, check: is the LiPo battery connected? Try disconnecting battery |
| 6 | Measure +3V3 test point with multimeter — should be 3.3V ±0.1V |
| 7 | Measure +5V test point — should be 4.8V–5.2V with USB plugged in |

### A5.2 — "Board works on USB but dies on battery"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Battery not charged | Measure battery voltage — should be 3.7V–4.2V | Charge via USB for 1+ hour |
| Battery disconnected | Check JST connector | Reconnect battery securely |
| Battery depleted/damaged | Battery voltage < 3.0V | Replace battery. LiPo cells permanently damaged below 3.0V |
| Schottky diode path issue | Battery voltage present at connector but not reaching +5V rail | Hardware issue — check Schottky diode on power path |

### A5.3 — "Polyfuse keeps tripping"

The polyfuse trips when current draw exceeds ~500mA. Common causes:

| Cause | Fix |
|-------|-----|
| Servo drawing from +3V3 rail | ⚠️ Move servo power to external 5V supply |
| Short circuit on breadboard | Check all wiring. Look for bare wires touching |
| Too many LEDs/peripherals | Calculate total current draw |
| Faulty peripheral (shorted sensor) | Disconnect peripherals one by one to find the culprit |

### A5.4 — "Board resets when servo moves"

This is the **classic servo brownout**. The servo draws a huge current spike when it starts moving, dropping the +5V rail below the 3.3V regulator's minimum input, causing the ESP32 to reset.

**Fix:** Power the servo from an external 5V supply (separate from USB). Connect servo signal (GPIO39) to the CoreV2, but servo power (+ and -) to the external supply. Connect the grounds together.

```
External 5V Supply (+) → Servo Red Wire
External 5V Supply (-) → Servo Brown Wire ─┬─→ CoreV2 GND (common ground)
                                            │
CoreV2 GPIO39          → Servo Orange/Yellow Wire (signal)
```

---

## A6. Serial Monitor Issues

### A6.1 — "Serial Monitor shows nothing"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Wrong baud rate | Serial Monitor baud rate doesn't match `Serial.begin()` | Both must be **115200** |
| USB CDC not enabled | `Serial` object doesn't route to USB | Tools → USB CDC On Boot → **Enabled** |
| `Serial.begin()` not called | Forgot to initialize Serial | Add `Serial.begin(115200);` in `setup()` |
| `while(!Serial)` blocking | This waits forever if USB CDC is wonky | Remove `while(!Serial);` or add timeout: `uint32_t t=millis(); while(!Serial && millis()-t<3000);` |
| Serial Monitor on wrong port | Multiple COM ports available | Check Tools → Port → select the ESP32-S3 port |

### A6.2 — "Serial output is garbled / random characters"

| Cause | Fix |
|-------|-----|
| Baud rate mismatch | Match Serial Monitor baud rate to `Serial.begin()` value (BOTH 115200) |
| ESP32 printing before Serial Monitor connects | Add `delay(1000);` after `Serial.begin()` in setup |
| Buffer overflow | Reduce print frequency. Don't print inside tight loops without delays |
| Wrong USB mode | Tools → USB Mode → **Hardware CDC and JTAG** |

### A6.3 — "Serial.print() inside loop() slows everything down"

Serial printing at 115200 baud sends ~11,520 characters/second. Printing a 100-character debug line every loop iteration at 60 FPS = 6,000 chars/sec = uses 52% of serial bandwidth, causing frame drops.

**Fix:** Throttle debug output:
```cpp
static uint32_t lastDebug = 0;
if (millis() - lastDebug > 1000) {  // Print once per second max
  Serial.printf("FPS: %d, State: %d\n", fps, state);
  lastDebug = millis();
}
```

---

## A7. Boot Loops, Crashes & Watchdogs

### A7.1 — "Board keeps rebooting" (Boot Loop)

```
Symptom: Serial Monitor shows repeating boot messages:
  "ets Jul 29 2019 12:21:46"
  "rst:0x1 (POWERON_RESET),boot:0x8 (SPI_FAST_FLASH_BOOT)"
  ...then repeats forever.
```

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| GPIO22-37 used as GPIO | ⛔ Most common cause | Remove all pinMode/digitalWrite to GPIO22-37 |
| Stack overflow | Large local arrays in functions | Move arrays to global scope or use `static` or heap (`malloc`) |
| Heap corruption | Writing past array bounds | Check all array accesses. Use `assert()` or bounds checks |
| Watchdog timeout | Loop or task taking too long | Add `delay(1)` or `yield()` in long-running loops. Feed watchdog: `esp_task_wdt_reset()` |
| PSRAM misconfigured | Settings say "OPI PSRAM" but `ESP.getFreePsram()` returns 0 | Tools → PSRAM → **OPI PSRAM**. Re-flash. |
| Power supply sag | Voltage drops below 3.0V during WiFi transmission | Use shorter USB cable. Check power supply can deliver 500mA+ |
| Faulty flash/PSRAM | Hardware defect | Run production test firmware (`CoreV2_ProductionTest.ino`) |

### A7.2 — "Brownout detector triggered"

```
Symptom: Serial shows "Brownout detector was triggered"
```

The ESP32 detected the +3V3 rail dropping below ~2.7V. This usually happens when:

1. **Servo draws too much current** — use external 5V supply
2. **WiFi transmission spike** — normal, but indicates marginal power supply
3. **USB cable too long/thin** — voltage drops over cable length. Use shorter, thicker cable.
4. **Battery low** — LiPo below 3.5V can't maintain stable 3.3V regulation

### A7.3 — "Guru Meditation Error"

```
Symptom: "Guru Meditation Error: Core 0 panic'ed (LoadProhibited)"
```

This is an ESP32 hardware exception. Common causes on CoreV2:

| Error Type | Meaning | Common Cause |
|-----------|---------|-------------|
| `LoadProhibited` | Reading from invalid memory address | NULL pointer dereference, accessing freed memory |
| `StoreProhibited` | Writing to invalid memory address | Writing to PROGMEM data, buffer overflow, writing to input-only GPIO34-39 |
| `InstrFetchProhibited` | Executing code from invalid address | Function pointer corruption, stack corruption |
| `IntegerDivideByZero` | Self-explanatory | Check all division operations for denominator=0 |
| `IllegalInstruction` | Corrupted code in flash | Re-flash firmware. If persists, flash may be damaged |

**Decode the backtrace:**
```bash
# In terminal (macOS/Linux), from sketch folder:
python3 ~/Library/Arduino15/packages/esp32/tools/esptool_py/*/esptool.py \
  --chip esp32s3 elf2image --output /dev/null your_sketch.ino.elf

# Or use: Tools → Exception Decoder in Arduino IDE 2.x
# Paste the "Backtrace:" lines from Serial Monitor
```

---

## A8. Library & Dependency Hell

### A8.1 — "Multiple libraries found for 'Adafruit_SSD1306.h'"

Arduino IDE picks one version arbitrarily. If you have multiple copies, the wrong one might be used.

**Fix:**
```
1. Close Arduino IDE
2. Navigate to: ~/Documents/Arduino/libraries/
3. Delete ALL folders starting with "Adafruit_SSD1306"
4. Also delete duplicates of "Adafruit_GFX", "Adafruit_BusIO"
5. Reopen Arduino IDE
6. Sketch → Include Library → Manage Libraries
7. Install ONLY ONE copy of each library
```

### A8.2 — Version Incompatibilities

| Problem | Fix |
|---------|-----|
| Adafruit SSD1306 2.5.x works, 2.6.x doesn't (or vice versa) | Install specific version from Library Manager dropdown |
| ESPAsyncWebServer needs AsyncTCP | Install **AsyncTCP** by me-no-dev, same version channel |
| IRremote 4.x breaks old code | Pin to IRremote 3.x if using NEC protocol examples |
| SD library conflicts | Use built-in SD (ESP32 core), NOT the "SD" library from Library Manager |

### A8.3 — "Library uses deprecated functions"

ESP32 Arduino core 3.x changed significant APIs. If you see deprecation warnings:

```cpp
// Old (core 2.x):
ledcSetup(channel, freq, resolution);
ledcAttachPin(pin, channel);

// New (core 3.x) — still works but with warning:
// Just ignore the warning, or use the new API:
// ledcAttach(pin, freq, resolution);  // All in one call
```

---

## A9. Memory Exhaustion (Heap & Stack)

### A9.1 — "malloc() failed" / "Heap exhausted"

```cpp
// Add this diagnostic to any project:
void printMemory() {
  Serial.printf("Free heap:     %6d bytes\n", ESP.getFreeHeap());
  Serial.printf("Free PSRAM:    %6d bytes\n", ESP.getFreePsram());
  Serial.printf("Min free heap: %6d bytes\n", ESP.getMinFreeHeap());
  Serial.printf("Heap size:     %6d bytes\n", ESP.getHeapSize());
}
```

| Cause | Fix |
|-------|-----|
| Large local arrays (`uint8_t buf[8000]`) | Move to global scope, use `static`, or allocate with `malloc()`/`new` |
| String concatenation in loop (`str += "more"`) | Use `snprintf()` into pre-allocated buffer, or `String::reserve()` |
| Memory leak (allocating without freeing) | Check all `malloc()` have matching `free()`. All `new` have matching `delete` |
| Web server buffers too large | Reduce `ASYNCWEBSERVER_MAX_BUFFER_SIZE` or use streaming responses |
| SD card library uses heap | SD library allocates ~50KB internally. If low on heap, only init SD when needed |

### A9.2 — "Stack overflow"

ESP32 main task has ~4KB stack by default. Large local variables or deep recursion will overflow.

```cpp
// ❌ BAD — 8000 bytes on stack:
void myFunction() {
  uint8_t hugeBuffer[8000];
}

// ✅ GOOD — on heap:
void myFunction() {
  uint8_t* hugeBuffer = (uint8_t*)malloc(8000);
  // ... use it ...
  free(hugeBuffer);
}

// ✅ GOOD — static (allocated once, not per-call):
void myFunction() {
  static uint8_t hugeBuffer[8000];
}
```

---

## A10. Compiler & Language Gotchas

### A10.1 — "`PROGMEM` doesn't work" / "Data in RAM instead of flash"

ESP32 uses a flat memory model (Harvard architecture but unified address space). `PROGMEM` is a no-op on ESP32 — data still goes to flash, but you can read it with normal pointers (no `pgm_read_byte()` needed).

```cpp
// On ESP32, both work identically:
const unsigned char data[] PROGMEM = {0x00, 0xFF};
uint8_t b1 = data[0];             // Works fine on ESP32!
uint8_t b2 = pgm_read_byte(&data[0]); // Also works, but unnecessary
```

### A10.2 — "`delay()` freezes my game loop"

`delay()` is **blocking** — nothing else runs while it waits. Use millis()-based timing instead:

```cpp
// ❌ BAD:
digitalWrite(LED, HIGH);
delay(500);
digitalWrite(LED, LOW);

// ✅ GOOD:
static uint32_t lastToggle = 0;
static bool ledState = false;
if (millis() - lastToggle >= 500) {
  ledState = !ledState;
  digitalWrite(LED, ledState);
  lastToggle = millis();
}
```

### A10.3 — "`map()` gives wrong values" / "Integer math losing precision"

`map()` uses integer math and truncates. For precise sensor mapping, use float:

```cpp
// ❌ BAD — loses precision:
int pct = map(analogRead(2), 2000, 500, 0, 100);  // Integer truncation

// ✅ GOOD — float preserves precision:
float pct = (analogRead(2) - 2000.0f) / (500.0f - 2000.0f) * 100.0f;
pct = constrain(pct, 0.0f, 100.0f);
```

### A10.4 — "`=` instead of `==` in condition"

The single most common student bug in C/C++:

```cpp
// ❌ BAD — assigns 5 to x, then checks if 5 is truthy (always true!):
if (x = 5) { /* always runs */ }

// ✅ GOOD — compares x to 5:
if (x == 5) { /* runs only when x equals 5 */ }

// 🛡️ DEFENSIVE — put constant on left (compiler catches typo):
if (5 == x) { /* if you type = instead of ==, compiler ERROR */ }
```

---

---

## B1. OLED Display Rendering (Game Boy)

### B1.1 — "Game graphics flicker / tear"

| Cause | Fix |
|-------|-----|
| `clearDisplay()` called every frame | Use `fillRect()` to clear only changed areas. Full `clearDisplay()` is slow over I²C |
| Drawing and `display()` interleaved | Batch ALL drawing operations, then call `display()` ONCE at end of frame |
| I²C clock too slow | Default Wire clock is 100kHz. Set to 400kHz: `Wire.setClock(400000);` |
| Frame rate too high for I²C bandwidth | Cap at 15 FPS. I²C at 400kHz can't push 128x64 pixels at 30 FPS |

```cpp
// ✅ CORRECT frame loop:
void loop() {
  // 1. Partial clear (only clear what moved)
  display.fillRect(oldPlayerX, oldPlayerY, 16, 16, SSD1306_BLACK);

  // 2. Draw everything
  drawPlayer(newPlayerX, newPlayerY);
  drawHUD();
  drawEnemies();

  // 3. ONE push to display
  display.display();  // ← only ONE of these per frame!

  // 4. Frame timing
  while (millis() - lastFrame < 66);  // Cap at ~15 FPS
  lastFrame = millis();
}
```

### B1.2 — "Image is shifted / offset / scrambled"

| Symptom | Cause | Fix |
|---------|-------|-----|
| Image shifted 2px right | SH1106 controller uses 132×64 RAM mapped to 128×64 display | Switch to SH1106 mode or add `x += 2` offset |
| Image shifted 2px left | SSD1306 code on SH1106 panel | Same as above |
| Every other column missing | Wrong byte orientation (horizontal vs vertical) | Ensure bitmap was generated with **Vertical - 1 bit per pixel** mode |
| Image inverted (black ↔ white) | Bits flipped | Use `SSD1306_BLACK` for draw color, or invert bytes: `data[i] = ~data[i]` |
| Static/garbage on screen | I²C communication error | Check pull-up resistors. Reduce I²C clock to 100kHz for testing |

### B1.3 — "OLED works for 5 minutes then dies"

| Cause | Fix |
|-------|-----|
| I²C bus lockup | Implement I²C watchdog: if display times out, re-init Wire |
| Overheating | OLED is rated for -40°C to +85°C. If burning hot to touch, there's a short |
| Power supply droop | Check +3V3 rail with multimeter. Should stay above 3.0V |
| Memory corruption from GPIO22-37 misuse | Check no student code touches these pins |

---

## B2. D-Pad & PCF8574 Button Input (Game Boy)

### B2.1 — "Buttons are reversed" / "Pressing UP triggers DOWN"

The PCF8574 is **active LOW** — a pressed button reads as 0. The bit must be inverted.

```cpp
// The cardinals are wrong because the D-Pad is physically rotated:
//   What the code calls "UP" might actually be "LEFT" on the PCB
// Always verify with the diagnostic sketch in Section A4.4
```

### B2.2 — "Holding a button registers as rapid presses" (no debounce)

```cpp
// ❌ BAD — no debounce:
if (digitalRead(BTN) == LOW) { doAction(); }

// ✅ GOOD — 50ms debounce:
static uint32_t lastPress[8] = {0};
if (pressed & bit) {
  if (millis() - lastPress[bit] > 50) {
    lastPress[bit] = millis();
    doAction();  // Only fires once per press
  }
}
```

### B2.3 — "Two buttons pressed at once only registers one"

The PCF8574 supports any combination of simultaneous button presses (each bit is independent). If it doesn't work:

| Cause | Fix |
|-------|-----|
| Code checks with `if/else if` chain | Use independent `if` for each button, not `else if` |
| Shadow register conflict (LEDs) | When writing LEDs, button bits must stay as 1 (input with pull-up). Use shadow register pattern |
| Electrical issue | When two buttons share a damaged trace, pressing both shorts the bus |

---

## B3. Analog Joystick (Game Boy)

### B3.1 — "Joystick drifts" / "Character moves on its own"

| Cause | Fix |
|-------|-----|
| Deadzone too small | Increase deadzone. Measured center values: X=1885, Y=1918. Deadzone: ±200 |
| ADC noise | Average 4-8 readings. Apply exponential smoothing (α=0.1–0.3) |
| Mechanical wear | Joystick potentiometers wear over time. Recalibrate center values |
| GPIO4/5 shared with encoder | If encoder is also connected, it pulls the ADC signal off | Use only joystick OR encoder, never both on GPIO4/5 |

### B3.2 — "Joystick only reads 0 or 4095"

| Cause | Fix |
|-------|-----|
| `analogReadResolution()` not set | Default is 12-bit on ESP32-S3. Explicitly set: `analogReadResolution(12);` |
| Wrong attenuation | Default attenuation limits range to ~1.1V. Use `analogSetAttenuation(ADC_11db);` |
| Pin configured as digital | `pinMode(4, OUTPUT)` will break analog reading. Remove it |
| ADC2 conflict with WiFi | GPIO4/5/6/7 are on ADC1 (no conflict). But GPIO11-20 are on ADC2 — WiFi blocks ADC2 | Use ADC1 pins (GPIO1-10) for analog when WiFi is active |

---

## B4. Buzzer Sound Effects (Game Boy)

### B4.1 — "Buzzer is silent"

Check the signal path: GPIO10 → Schottky D6 → Q2 base → Q2 collector → BZ1 → R7 (330Ω) → +5V

| Step | Test |
|------|------|
| 1 | Does GPIO10 output PWM? Connect LED+resistor to GPIO10 (temporarily) — should blink |
| 2 | Is Q2 getting base current? Measure voltage at Q2 base — should be ~0.7V when GPIO10 is HIGH |
| 3 | Is +5V reaching buzzer? Measure at buzzer positive terminal — should be 5V |
| 4 | Is buzzer working? Briefly touch buzzer to 5V and GND — should click |

### B4.2 — "Buzzer is very quiet"

| Cause | Fix |
|-------|-----|
| PWM duty cycle too low | Default `ledcWrite()` with 10-bit resolution: 0-1023. Use `ledcWrite(1, 512);` for 50% duty |
| Wrong frequency for buzzer resonance | Piezo buzzers are loudest at their resonant frequency (typically 2-4 kHz). Try 2000-4000 Hz |
| R7 too large (current limiting too aggressive) | R7 is 330Ω on CoreV2 — this limits current to ~15mA. If buzzer needs more, external amplifier needed |
| Q2 not saturating | Base resistor R21 (10kΩ) may be too large. Check Q2 Vce — should be < 0.3V when on |

### B4.3 — "Buzzer makes clicking noise instead of tone"

| Cause | Fix |
|-------|-----|
| PWM channel not configured | Must call `ledcSetup(1, 440, 10);` before `ledcWrite()` or `ledcWriteTone()` |
| Wrong LEDC channel | Buzzer is on LEDC channel 1. Servo is channel 0. Don't mix them up |
| GPIO not attached to LEDC | Must call `ledcAttachPin(10, 1);` to connect GPIO10 to channel 1 |
| Using `ledcWrite()` for variable frequency | `ledcWrite()` sets duty, not frequency. Use `ledcWriteTone(ch, freq)` for variable frequency output |

---

## B5. microSD Card Save/Load (Game Boy)

### B5.1 — "SD card not detected" / "SD.begin() fails"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Card not formatted FAT32 | Windows: Right-click → Format → FAT32. Mac: Disk Utility → Erase → MS-DOS (FAT) | Reformat card |
| Card > 32GB | SDHC/SDXC cards > 32GB use exFAT by default | Use 4-32GB card formatted FAT32 |
| SPI pins wrong | CoreV2 uses VSPI: MOSI=11, MISO=13, CLK=12, CS=14 | Use `SPI.begin(12, 13, 11); SD.begin(14);` |
| Card detect pin (GPIO21) | `digitalRead(21)` should be LOW when card inserted | Check if card detect switch is working |
| Card locked (write-protect tab) | Physical tab on SD card in LOCK position | Slide tab to unlock |
| Pin conflict | GPIO11-14 used for something else in code | Check no other code uses these pins |

### B5.2 — "Can write to SD but can't read back"

| Cause | Fix |
|-------|-----|
| File not closed | Always `file.close();` after writing. Unclosed files may have 0 bytes |
| Wrong file mode | `SD.open("file.txt", "r")` for read, `"w"` for write (overwrites), `"a"` for append |
| File name too long | SD library uses 8.3 filenames. Keep names ≤ 8 chars + 3 char extension |
| File in wrong directory | `SD.open("/data/save.dat")` vs `SD.open("save.dat")` — be explicit with path |

---

## B6. Game Logic & State Machines (Game Boy)

### B6.1 — "Game freezes" / "Unresponsive"

| Cause | Fix |
|-------|-----|
| Infinite loop in game logic | Add watchdog checks. Use `while(condition) { if(millis()-start>timeout) break; }` |
| Blocking `delay()` in game loop | Replace ALL `delay()` calls with timer-based patterns |
| Stack overflow in recursive function | Limit recursion depth. Use iterative approaches for pathfinding |
| Deadlock waiting for resource | If using mutex/semaphore, ensure they're always released |

### B6.2 — "State machine gets stuck"

```cpp
// ✅ DEFENSIVE state machine — always has an escape:
void updateState() {
  static uint32_t stateEntered = millis();
  switch (state) {
    case LOADING:
      if (assetsLoaded) { state = TITLE; stateEntered = millis(); }
      if (millis() - stateEntered > 10000) { state = ERROR; }  // Timeout escape!
      break;
    // ... etc
  }
}
```

---

## B7. Animation & Sprites on OLED (Game Boy)

### B7.1 — "Animation doesn't play" / "Shows garbage"

| Cause | Fix |
|-------|-----|
| Wrong array index | `animation[frame]` — check `frame < ANIM_FRAMES` |
| Wrong byte offset | `drawBitmap(x, y, &animation[frame][0], w, h, WHITE)` — use correct sub-array |
| Width/height mismatch | Constants must match the bitmap data exactly. 32×32 = 128 bytes |
| PROGMEM on AVR but not ESP32 | On ESP32, PROGMEM is no-op but data IS in flash. Normal pointers work fine |
| Frame timing too fast | Visible as flicker. Increase `FRAME_DELAY_MS` to 80-100ms |

### B7.2 — "Animation area flashes white"

This happens when `clearDisplay()` is called between frames. Use partial clear:

```cpp
// ❌ BAD — entire screen flickers:
display.clearDisplay();
display.drawBitmap(x, y, anim[frame], 32, 32, SSD1306_WHITE);
display.display();

// ✅ GOOD — only animation area cleared:
display.fillRect(x, y, 32, 32, SSD1306_BLACK);  // Clear just the animation zone
display.drawBitmap(x, y, anim[frame], 32, 32, SSD1306_WHITE);
display.display();
```

---

---

## C1. Capacitive Soil Moisture Sensor (Smart Plant Pot)

### C1.1 — "Sensor reading doesn't change between wet and dry"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Wrong sensor type | Student using resistive sensor (two bare electrodes) instead of capacitive | Capacitive sensors have a coated PCB. Resistive sensors corrode and die quickly |
| Sensor not in soil | Test: sensor in open air → should read HIGH. Sensor in water → should read LOW (capacitive) or HIGH (resistive) | Calibrate with known conditions |
| Sensor VCC wrong | Most capacitive sensors work at 3.3V or 5V — check datasheet | Match VCC to sensor spec |
| ADC pin not reading | Test with multimeter: analog output should change voltage between dry/wet | Check wiring. Try a different GPIO (GPIO2 or GPIO41/42) |

### C1.2 — "Sensor readings jump around randomly"

| Cause | Fix |
|-------|-----|
| ADC noise (ESP32 ADC is notoriously noisy) | Average 10-20 readings. Add 100ms delay between readings for ADC to settle |
| Electrical interference | Keep sensor wires away from WiFi antenna and servo wires |
| Poor ground connection | Ensure sensor GND connects directly to CoreV2 GND |
| Power supply noise | Add 100μF capacitor between sensor VCC and GND |

### C1.3 — "Sensor corroded / stopped working after a week"

| Cause | Fix |
|-------|-----|
| Using resistive sensor (not capacitive) | Replace with capacitive soil moisture sensor (v1.2 PCB style) |
| Sensor constantly powered | Moisture + DC voltage = electrolysis. Only power sensor when reading: connect VCC to GPIO pin, set HIGH before read, LOW after |
| Sensor left in wet soil continuously | Remove sensor from soil when not actively monitoring. Clean with isopropyl alcohol |

### C1.4 — "Sensor works backward (wet = low, dry = high)"

**This is NORMAL for capacitive sensors.** Capacitive sensors measure dielectric constant — water has higher dielectric than air, so wet soil = higher capacitance. Depending on the circuit design, this can translate to either higher or lower voltage.

**Fix:** Calibrate for YOUR specific sensor. Don't assume polarity — test both wet and dry, then adjust `map()` accordingly.

```cpp
// If wet=low, dry=high:
int pct = map(raw, DRY_VALUE, WET_VALUE, 0, 100);

// If wet=high, dry=low:
int pct = map(raw, WET_VALUE, DRY_VALUE, 0, 100);
```

---

## C2. NTC Thermistor Temperature (Smart Plant Pot)

### C2.1 — "Temperature reading is wildly inaccurate"

CoreV2 uses a 10kΩ NTC thermistor (TH1) with β=3950 in a voltage divider with R6=10kΩ.

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Wrong β value | Used β=3435 or other value instead of β=3950 | Use `3950.0f` in calculation |
| Wrong R25 value | Used 100kΩ instead of 10kΩ | `R25 = 10000.0f` |
| Self-heating | Reading too frequently causes thermistor to warm itself | Read at most once per second |
| Wrong fixed resistor value | R6 is 10kΩ. If it's actually a different value... | Verify R6 with multimeter |
| ADC not calibrated | ESP32 ADC has ±6% variance between chips | Add calibration offset if precision needed |

### C2.2 — "Temperature reads -273°C" (absolute zero / math error)

This means the Steinhart-Hart calculation produced a division by zero or log of negative.

```cpp
// ✅ DEFENSIVE temperature calculation:
float readTemperature() {
  int raw = analogRead(1);
  if (raw == 0 || raw >= 4095) return NAN;  // Avoid divide-by-zero

  float voltage = (raw / 4095.0f) * 3.3f;
  float resistance = (3.3f * 10000.0f / voltage) - 10000.0f;
  if (resistance <= 0) return NAN;  // Avoid log of negative

  float steinhart = log(resistance / 10000.0f) / 3950.0f;
  steinhart += 1.0f / 298.15f;
  if (steinhart == 0) return NAN;  // Avoid divide-by-zero

  return (1.0f / steinhart) - 273.15f;
}
```

---

## C3. ADC Reading & Noise (Smart Plant Pot)

### C3.1 — "analogRead() returns 4095 for everything"

| Cause | Fix |
|-------|-----|
| Pin floating (disconnected) | ESP32 ADC floats HIGH when disconnected. Connect sensor or use pull-down |
| Wrong attenuation | Default 0dB attenuation clamps at ~1.1V. For 3.3V sensors: `analogSetAttenuation(ADC_11db)` |
| Sensor output > 3.3V | Will damage ESP32! Use voltage divider if sensor outputs 5V |

### C3.2 — "ADC values inconsistent between readings"

ESP32-S3 ADC is known to be nonlinear and noisy. Mitigation strategies:

```cpp
// ✅ ROBUST ADC reading with outlier rejection:
int readADCStable(int pin, int samples = 20) {
  int readings[samples];
  for (int i = 0; i < samples; i++) {
    readings[i] = analogRead(pin);
    delay(5);
  }
  // Sort and take median of middle 60%
  std::sort(readings, readings + samples);
  long sum = 0;
  int start = samples * 0.2;  // Skip lowest 20%
  int end = samples * 0.8;    // Skip highest 20%
  for (int i = start; i < end; i++) sum += readings[i];
  return sum / (end - start);
}
```

---

## C4. Threshold Alerts & Hysteresis (Smart Plant Pot)

### C4.1 — "Alert triggers on and off rapidly at threshold boundary"

This is called **chattering** or **hunting**. Fix with hysteresis:

```cpp
// ❌ BAD — no hysteresis:
if (moisture < 30) { alertOn(); }
else { alertOff(); }

// ✅ GOOD — 5% hysteresis band:
static bool alertActive = false;
if (moisture < 25 && !alertActive) { alertOn();  alertActive = true; }   // Activate at 25%
if (moisture > 35 && alertActive)  { alertOff(); alertActive = false; }  // Deactivate at 35%
```

### C4.2 — "Alert never triggers" / "Alert always on"

| Cause | Fix |
|-------|-----|
| Threshold not calibrated | Calibrate moisture% mapping first (Section C1.4) |
| Comparison direction wrong | `if (moisture < threshold)` vs `if (moisture > threshold)` — depends on sensor polarity |
| Alert check only runs once | Put alert check in `loop()`, not in `setup()` |
| Variable scope issue | Threshold variable declared locally (re-initialized every loop) instead of globally |

---

## C6. OLED Plant Dashboard (Smart Plant Pot)

### C6.1 — "Text overwrites itself" / "Garbage characters"

| Cause | Fix |
|-------|-----|
| Not clearing display between updates | Call `display.clearDisplay()` at start of each dashboard update |
| `display.print()` without `setCursor()` | Text cursor continues from where last `print()` left off. Always set cursor position |
| Printing numbers without formatting | `display.print(temp)` may print "23.459999" — use `display.printf("%.1f", temp)` |
| Text size changes without resetting | After `setTextSize(2)`, call `setTextSize(1)` before next `print()` |
| Buffer overflow in `printf` | `display.printf()` uses a fixed buffer. Long strings get truncated. Use multiple `print()` calls |

---

## C7. WiFi IoT Dashboard (Smart Plant Pot, Optional)

### C7.1 — "WiFi connects then disconnects"

| Cause | Fix |
|-------|-----|
| WiFi and ADC2 conflict | WiFi uses ADC2. Moisture sensor on GPIO2 (ADC1) is fine, but if you moved it to an ADC2 pin, it conflicts | Use ADC1 pins (GPIO1-10) when WiFi is active |
| Power sag during TX | WiFi transmission draws ~300mA spike. Use good USB cable and power supply |
| Router too far | ESP32 WiFi range ~50m open air, ~10m through walls | Move closer to router |
| 5GHz network (ESP32 is 2.4GHz only) | ESP32 can't see 5GHz networks at all | Use 2.4GHz SSID or enable both bands on router |

---

---

## D1. Continuous Rotation Servo (Smart Safe)

### D1.1 — "Servo doesn't move at all"

| Step | Test |
|------|------|
| 1 | Servo powered? Measure voltage at servo connector: should be 4.8-6V |
| 2 | Signal reaching servo? LEDC output on GPIO39: check with `ledcWrite(0, 307);` |
| 3 | Servo wired correctly? Signal=GPIO39(yellow/orange), Power=+5V(red), GND=GND(brown/black) |
| 4 | Common ground? Servo GND MUST connect to CoreV2 GND |
| 5 | Servo not defective? Test with servo tester or known-good Arduino |

### D1.2 — "Servo moves, but wrong direction"

```cpp
// Continuous rotation servo direction depends on pulse width:
// 1.5ms = STOP (duty=307 at 50Hz, 12-bit)
// < 1.5ms = one direction (duty 205-306)
// > 1.5ms = other direction (duty 308-410)

// If directions are swapped, swap the duty values:
#define SERVO_CW   266   // Was 348
#define SERVO_CCW  348   // Was 266
```

### D1.3 — "Servo doesn't stop" / "Creeps slowly"

| Cause | Fix |
|-------|-----|
| STOP pulse not exactly 1.5ms | Every servo is slightly different. Find the true STOP pulse by incrementally adjusting 300-315 |
| Signal noise | Keep servo signal wire away from motor power wires |
| Low voltage | If supply voltage sags under load, servo may drift | Use stable external 5V supply |

### D1.4 — "Servo rotates more/less than expected"

With continuous rotation servos, rotation angle = rotation speed × time:

```cpp
// Calibrate by measuring: "how many ms to rotate exactly 90°?"
// Default: 1500ms, but this varies per servo
#define SERVO_90_DEG_MS  1500  // Tune this value!

void rotate90Degrees(bool clockwise) {
  uint16_t duty = clockwise ? 348 : 266;
  ledcWrite(0, duty);
  delay(SERVO_90_DEG_MS);
  ledcWrite(0, 307);  // STOP
}
```

### D1.5 — "Servo twitches on startup"

| Cause | Fix |
|-------|-----|
| LEDC not initialized before attach | Call `ledcSetup()` → `ledcWrite(0, 307)` (STOP) → `ledcAttachPin(39, 0)` |
| Power-on surge | Add 100ms delay before first servo command |
| Floating signal during boot | GPIO39 may float before `pinMode()` is called. Add 10kΩ pull-down resistor from GPIO39 to GND |

---

## D2. Servo Power Supply (Smart Safe)

### D2.1 — THE MOST CRITICAL SERVO RULE

```
⛔ NEVER POWER THE SERVO FROM THE COREV2 +3V3 RAIL ⛔

A servo under load can draw 500mA–2A.
The CoreV2 +3V3 regulator can supply maybe 500mA total.
The ESP32 needs ~80mA. OLED needs ~20mA. PCF8574 needs ~1mA.

SERVO + COREV2 3V3 RAIL = IMMEDIATE BROWNOUT RESET
```

**Correct wiring:**
```
External 5V 2A Power Supply
  ├── (+) → Servo Red Wire
  ├── (-) → Servo Brown/Black Wire ──→ CoreV2 GND (common ground!)
  │
CoreV2 GPIO39 → Servo Orange/Yellow Wire (signal only!)
```

### D2.2 — "Using a battery pack for servo — how long will it last?"

| Battery | Capacity | Servo Active | Servo Idle |
|---------|----------|-------------|------------|
| 4×AA alkaline | ~2000 mAh @ 6V | ~2-3 hours | ~20 hours |
| USB power bank | 5000 mAh @ 5V | ~5-8 hours | ~50 hours |
| 2S LiPo (7.4V) | 1000 mAh | ⚠️ Need voltage regulator — 7.4V will kill servo | — |

---

## D3. PIN Entry UI & Debounce (Smart Safe)

### D3.1 — "One button press enters multiple digits"

This is **switch bounce**. Mechanical switches bounce for 5-50ms when pressed.

```cpp
// ✅ DEBOUNCED button reading:
bool buttonPressed(uint8_t btn, uint8_t bit) {
  static uint32_t lastPress[8] = {0};
  static bool lastState[8] = {false};

  bool current = (btn & (1 << bit)) != 0;

  if (current && !lastState[bit]) {
    if (millis() - lastPress[bit] > 50) {  // 50ms debounce
      lastPress[bit] = millis();
      lastState[bit] = current;
      return true;
    }
  }
  lastState[bit] = current;
  return false;
}
```

### D3.2 — "Holding a button repeats rapidly" (auto-repeat)

Sometimes you WANT auto-repeat (like holding UP to scroll through digits quickly):

```cpp
bool buttonPressedOrRepeating(uint8_t btn, uint8_t bit, uint32_t repeatDelay = 500) {
  static uint32_t lastPress[8] = {0};
  static bool wasHeld[8] = {false};
  bool current = (btn & (1 << bit)) != 0;

  if (current) {
    if (millis() - lastPress[bit] > 50) {  // Debounce
      if (!wasHeld[bit] || millis() - lastPress[bit] > repeatDelay) {
        lastPress[bit] = millis();
        wasHeld[bit] = true;
        return true;
      }
    }
  } else {
    wasHeld[bit] = false;
  }
  return false;
}
```

---

## D4. Authentication & Security (Smart Safe)

### D4.1 — "PIN is stored in plain text" (Security Risk)

```cpp
// ❌ TERRIBLE — PIN visible in code and flash dump:
const char* PIN = "1234";

// ✅ BETTER — store hash, not PIN:
// Use SHA-256 hash of PIN+salt. Compare hashes, not plaintext.
// Even if flash is dumped, PIN can't be recovered.
// (For camp: plain text is OK for learning, but explain the risk)
```

### D4.2 — "Lockout doesn't work" / "Can brute-force forever"

```cpp
// ✅ PROPER LOCKOUT:
#define MAX_ATTEMPTS 5
#define LOCKOUT_MS   30000  // 30 seconds

uint8_t failedAttempts = 0;
uint32_t lockoutUntil = 0;

bool tryUnlock(const char* enteredPin) {
  if (millis() < lockoutUntil) {
    Serial.printf("Locked out! %d seconds remaining\n",
                  (lockoutUntil - millis()) / 1000);
    return false;
  }

  if (strcmp(enteredPin, correctPin) == 0) {
    failedAttempts = 0;
    return true;  // Success!
  }

  failedAttempts++;
  if (failedAttempts >= MAX_ATTEMPTS) {
    lockoutUntil = millis() + LOCKOUT_MS;
    failedAttempts = 0;
    Serial.println("🔒 LOCKOUT — too many failed attempts!");
  }
  return false;
}
```

### D4.3 — "Timing attack possible" (Advanced)

If PIN verification uses `strcmp()`, an attacker can measure response time to guess digits one at a time. For camp, this is beyond scope — but mention it as a "real world" concern.

---

## D5. LittleFS Config Storage (Smart Safe)

### D5.1 — "Config lost after power cycle" / "Reads default values"

| Cause | Fix |
|-------|-----|
| Partition scheme doesn't include LittleFS | Tools → Partition Scheme must include "spiffs" or "LittleFS" | Use "Default 4MB with spiffs" or custom LittleFS partition |
| Config not written (only in RAM) | `saveConfig()` must be called after changes | Add auto-save after each config change |
| File not closed after write | `file.close()` flushes buffers to flash | Always close files! |
| Flash wear (too many writes) | Writing every loop iteration | Use dirty flag pattern — only write when data actually changed |

### D5.2 — "LittleFS.begin() fails" / "Corrupted filesystem"

```cpp
// Recovery: format if corrupted
if (!LittleFS.begin(false)) {  // false = don't format
  Serial.println("LittleFS corrupted! Formatting...");
  if (!LittleFS.begin(true)) {  // true = format
    Serial.println("❌ Format failed — flash may be damaged");
  }
}
```

---

## D6. State Machine Logic (Smart Safe)

### D6.1 — "Safe unlocks without PIN" / "Skips states"

```cpp
// ❌ BAD — no transition validation:
if (digitalRead(UNLOCK_BTN)) { state = UNLOCKED; }

// ✅ GOOD — only unlock from valid states:
if (digitalRead(UNLOCK_BTN) && state == LOCKED) {
  // Unlock button does nothing when already unlocked!
  // Only transition from LOCKED → UNLOCKED via AUTHENTICATED
}
```

Always draw the state diagram before coding:
```
LOCKED → (CENTER) → ENTERING_PIN → (correct) → UNLOCKED
                  → (wrong)     → LOCKED (increment attempts)
                  → (5 wrong)   → LOCKOUT → (timeout) → LOCKED
UNLOCKED → (CENTER) → LOCKED
```

---

---

## E1. PIR Motion Sensor (HC-SR501 / AM312) (Security System)

### E1.1 — "PIR triggers constantly / false alarms"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Warmup not complete | PIR needs 30-60 seconds to stabilize after power-on | Add `delay(30000);` after power-on before reading |
| Sensitivity too high | Adjustment pot turned to max | Turn sensitivity pot counterclockwise to reduce range |
| Sensor sees heat source | Sunlight, heater vent, or air conditioner in field of view | Reposition sensor away from environmental heat sources |
| Sensor sees moving shadows | Sunlight through window casting moving shadows | PIR detects IR changes — shadows from moving clouds will trigger it |
| Electrical noise | Long wires act as antennas | Keep PIR wires < 30cm. Use twisted pair or shielded cable |
| Time delay too short | PIR re-triggers immediately after timeout | Turn time-delay pot clockwise to increase (1-10 seconds) |

### E1.2 — "PIR doesn't trigger at all"

| Step | Test |
|------|------|
| 1 | Power LED on PIR module? Check VCC (5V) and GND |
| 2 | Cover sensor with hand for 5 seconds, then remove — should trigger |
| 3 | Measure PIR output pin with multimeter — should go HIGH (3.3V) on motion |
| 4 | Try different PIR module (they do fail sometimes) |
| 5 | Bypass PIR: connect GPIO41 to 3.3V — should read HIGH | If not, GPIO41 is misconfigured |
| 6 | Check: `pinMode(41, INPUT);` — without this, `digitalRead()` returns unpredictable values |

### E1.3 — "AM312 vs HC-SR501 — which to use?"

| Feature | HC-SR501 | AM312 |
|---------|----------|-------|
| Size | Large (3.2×2.4cm) | Tiny (1.2×1.2cm) |
| Voltage | 5V-20V (5V typical) | 2.7V-12V (3.3V OK!) |
| Current | ~65μA idle | ~30μA idle |
| Range | 3-7 meters | 3-5 meters |
| Adjustments | 2 pots (sensitivity, time) | Fixed (no pots) |
| Lens | Fresnel dome | Integrated lens |
| CoreV2 compatibility | Needs 5V (use VBUS) | Can use 3.3V directly! |

**Recommendation for CoreV2:** AM312 is easier — no pots to misadjust, runs on 3.3V. HC-SR501 gives more control but needs 5V.

---

## E2. IR Receiver & Transmitter Beam (Security System)

### E2.1 — "IR receiver never detects signal"

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Transmitter LED not actually emitting | IR is invisible to human eye! Use phone camera to see IR LED glow (phone cameras see IR as purple/white) | Check with phone camera |
| Wrong carrier frequency | Most IR receivers are 38kHz. Some are 36kHz, 40kHz | Check receiver part number. CoreV2 uses 38kHz |
| IR LED polarity reversed | IR LED has polarity (like all LEDs). Longer leg = anode (+) | Flip LED orientation |
| No current-limiting resistor | IR LED needs ~100Ω resistor in series — otherwise burns out instantly | Add 100Ω resistor between GPIO38 and IR LED anode |
| Ambient IR interference | Sunlight contains massive amounts of IR. Fluorescent lights emit IR noise | Test in dim room. Add IR filter (dark red plastic) in front of receiver |
| Distance too far | Typical IR range: 1-5 meters with direct line of sight | Position TX and RX closer. Align them carefully |

### E2.2 — "IR beam works but is unreliable"

| Cause | Fix |
|-------|-----|
| Beam misalignment | TX and RX must point directly at each other. IR is line-of-sight only |
| Reflections | IR reflects off shiny surfaces — can create false "beam intact" readings | Use matte black material behind RX to absorb reflections |
| Interference from TV remotes | TV remote uses same 38kHz carrier | Use unique NEC code. Check received code matches expected before declaring "beam intact" |
| Duty cycle too high | IR LED running continuously overheats and dims | Pulse IR LED at 38kHz with 50% duty cycle for best range |

### E2.3 — "IRremote library won't compile"

IRremote library underwent major API changes in v4.x:

```cpp
// IRremote 3.x (old):
#include <IRremote.h>
IRrecv irrecv(40);
decode_results results;
if (irrecv.decode(&results)) { /* ... */ irrecv.resume(); }

// IRremote 4.x (new):
#include <IRremote.hpp>
IrReceiver.begin(40);
if (IrReceiver.decode()) {
  uint32_t code = IrReceiver.decodedIRData.decodedRawData;
  IrReceiver.resume();
}
```

---

## E3. ESP-NOW Wireless Mesh (Security System)

### E3.1 — "ESP-NOW messages not received"

ESP-NOW is a peer-to-peer protocol. Both devices must be configured correctly.

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| MAC addresses wrong | Print MAC on each device: `Serial.println(WiFi.macAddress());` | Use EXACT printed MAC (uppercase hex, colon-separated) |
| WiFi mode wrong | ESP-NOW needs WiFi in STA mode | `WiFi.mode(WIFI_STA);` before `esp_now_init()` |
| Not on same channel | ESP-NOW uses WiFi channel 1 by default | Set channel explicitly: `WiFi.begin(ssid, pass, 1);` then disconnect if you don't need internet |
| Peer not added | Sender must add receiver's MAC as peer | `esp_now_add_peer(&peerInfo);` |
| Encryption mismatch | If using encryption, both sides must use same PMK | For camp: don't use encryption. Simpler, easier to debug |
| WiFi interference | 2.4GHz crowded (many WiFi networks nearby) | ESP-NOW is resilient but extreme congestion can cause drops |

### E3.2 — "ESP-NOW peer add fails"

Common causes of `esp_now_add_peer()` failure:

```cpp
// ❌ Fails because channel is 0:
esp_now_peer_info_t peer = {};
memcpy(peer.peer_addr, mac, 6);
peer.channel = 0;  // Invalid! Must be 1-14

// ✅ Correct:
peer.channel = 1;  // Use WiFi channel 1
peer.encrypt = false;
esp_now_add_peer(&peer);
```

### E3.3 — "Messages arrive but data is corrupted"

```cpp
// ❌ DANGEROUS — different struct layouts on different compilers:
struct Message { int a; char b; int c; };  // Padding bytes may differ!

// ✅ SAFE — use packed struct:
#pragma pack(1)  // Or: __attribute__((packed))
struct Message { uint8_t zone; bool motion; uint8_t battery; };
#pragma pack()

// ✅ SAFER — use fixed-size array:
uint8_t msg[3] = {zone, motion, battery};
esp_now_send(mac, msg, 3);
```

---

## E4. ESP32-C3 Supermini Remote Nodes (Security System)

### E4.1 — "Can't program the C3 Supermini"

The C3 Supermini is a tiny ESP32-C3 board, not ESP32-S3. Different chip, different settings.

| Setting | ESP32-C3 Supermini | CoreV2 (ESP32-S3) |
|---------|-------------------|-------------------|
| Board | **ESP32C3 Dev Module** | ESP32S3 Dev Module |
| USB CDC On Boot | **Enabled** | Enabled |
| Flash Size | **4MB** | 16MB |
| PSRAM | **Disabled** (C3 Supermini has no PSRAM!) | OPI PSRAM |
| CPU Frequency | **160MHz** | 240MHz |
| Upload Mode | **Internal USB** | UART0 / Hardware CDC |

### E4.2 — "C3 Supermini GPIO differences"

```
⚠️ The C3 Supermini has DIFFERENT GPIO capabilities than ESP32-S3:
  - C3 has fewer pins (~11 usable)
  - C3 has different ADC pins
  - C3 strapping pins: GPIO2, GPIO8, GPIO9
  - NOT the same as S3 strapping pins!

Check the C3 Supermini pinout diagram before assigning pins!
```

### E4.3 — "C3 Supermini keeps disconnecting from WiFi"

| Cause | Fix |
|-------|-----|
| Antenna issue | C3 Supermini has a tiny ceramic antenna. Range is shorter than CoreV2 (~30m vs ~50m) |
| Power saving | C3 has aggressive power saving by default | Disable: `WiFi.setSleep(false);` |
| ESP-NOW on C3 | C3 supports ESP-NOW but has less TX power | Reduce distance between C3 node and CoreV2 hub |

---

## E5. Alarm Siren Logic (Security System)

### E5.1 — "Siren blocks everything else" / "Can't disarm during alarm"

```cpp
// ❌ BLOCKING — nothing else runs during alarm:
void soundAlarm() {
  for (int i = 0; i < 20; i++) {
    ledcWriteTone(1, 880); delay(300);
    ledcWriteTone(1, 440); delay(300);
  }
}

// ✅ NON-BLOCKING — alarm plays while system remains responsive:
void updateAlarm() {
  if (!alarmActive) return;

  // Auto-timeout after 10 seconds
  if (millis() - alarmStart > 10000) {
    alarmActive = false;
    ledcWriteTone(1, 0);
    return;
  }

  // Toggle siren tone every 300ms
  uint32_t phase = (millis() - alarmStart) % 600;
  if (phase < 300) ledcWriteTone(1, 880);
  else             ledcWriteTone(1, 440);
}

// In loop():
updateAlarm();
uint8_t btn = readButtons();
if (btn & (1<<1)) {  // DOWN button = DISARM
  alarmActive = false;
  ledcWriteTone(1, 0);
}
```

---

## E6. SD Card Event Logging (Security System)

### E6.1 — "Log file corrupted after power loss"

| Cause | Fix |
|-------|-----|
| File not closed before power-down | Always `file.close()` after each write. Don't keep file open for long periods |
| Write during power sag | Brownout during SD write = corrupted sector | Use battery or add large capacitor (1000μF) on +3V3 rail |
| Card removed while writing | SD card pull-out during active write = card corruption | Check card detect pin (GPIO21) before writing |

### E6.2 — "SD card fills up with log entries"

```cpp
// ✅ LOG ROTATION — keep only last N entries:
void logEvent(const char* event) {
  File f = SD.open("/security.log", FILE_APPEND);
  f.printf("[%lu] %s\n", millis(), event);
  f.close();

  // Check file size, rotate if > 100KB
  f = SD.open("/security.log", "r");
  if (f.size() > 100000) {
    f.close();
    SD.remove("/security.old");         // Delete old backup
    SD.rename("/security.log", "/security.old");  // Rotate
    // New log starts fresh on next write
  } else {
    f.close();
  }
}
```

---

## E7. Multi-Zone Coordination (Security System)

### E7.1 — "Zones interfere with each other" / "One zone triggers others"

Each zone should be independently monitored. Use a zone state array:

```cpp
#define MAX_ZONES 5

struct Zone {
  bool armed;
  bool triggered;
  uint32_t triggerTime;
};
Zone zones[MAX_ZONES];

void checkZone(uint8_t id) {
  if (!zones[id].armed) return;
  if (zones[id].triggered) return;  // Already triggered

  bool intrusion = false;
  switch (id) {
    case 0: intrusion = !digitalRead(40); break;    // IR RX (active LOW)
    case 1: intrusion = digitalRead(41);  break;     // PIR (active HIGH)
    case 2: intrusion = /* UART message */; break;
    case 3:
    case 4: intrusion = /* ESP-NOW message */; break;
  }

  if (intrusion) {
    zones[id].triggered = true;
    zones[id].triggerTime = millis();
    logEvent(id, "INTRUSION");
  }
}
```

---

---

## F1. Capacitive Moisture Sensor Deep Dive

### F1.1 — How It Actually Works

A capacitive moisture sensor measures the **dielectric constant** of the soil. Water has a dielectric constant of ~80, while dry soil is ~3-5. The sensor uses a 555 timer or similar oscillator circuit where the soil acts as the capacitor in an RC circuit. Wet soil = higher capacitance = different frequency/voltage.

### F1.2 — Resistive vs Capacitive (CRITICAL)

| Feature | Resistive Sensor | Capacitive Sensor |
|---------|-----------------|-------------------|
| Principle | Measures resistance between two electrodes | Measures capacitance change |
| Corrosion | **YES** — electrodes corrode rapidly in wet soil | **NO** — electrodes are coated/insulated |
| Lifespan | Days to weeks | Months to years |
| Accuracy | Drifts as electrodes corrode | Stable over time |
| Cost | $0.50 | $2-5 |
| Output | Analog voltage (0-3.3V or 0-5V) | Analog voltage (0-3.3V typical) |
| Appearance | Two bare metal prongs | Coated PCB with no exposed metal |

**For camp: ONLY use capacitive sensors.** Resistive sensors will die within a week and teach students bad lessons.

### F1.3 — Wiring Variations

Some capacitive sensors have 3 pins, some have 4:

```
3-pin version:
  VCC — Power (3.3V or 5V)
  GND — Ground
  AOUT — Analog output (0-3.3V)

4-pin version:
  VCC — Power
  GND — Ground
  AOUT — Analog output
  DOUT — Digital output (HIGH/LOW threshold, adjustable via pot)
```

For CoreV2, use the analog output (AOUT) on GPIO2.

### F1.4 — Preventing Corrosion (Even on Capacitive Sensors)

Even capacitive sensors can corrode at the connector end if moisture wicks up. Mitigations:

1. **Only power when reading:** Connect VCC to a GPIO pin. Set HIGH → read → set LOW
2. **Seal the connector:** Apply hot glue or silicone around the wire-to-PCB connection
3. **Remove when not in use:** Don't leave sensor in wet soil 24/7

```cpp
// ✅ Power sensor only when reading:
#define MOISTURE_PWR 41  // GPIO to power the sensor
#define MOISTURE_AOUT 2   // GPIO for analog reading

int readMoisture() {
  digitalWrite(MOISTURE_PWR, HIGH);  // Power on
  delay(10);                          // Stabilize
  int val = analogRead(MOISTURE_AOUT);
  digitalWrite(MOISTURE_PWR, LOW);   // Power off
  return val;
}
```

---

## F2. NTC Thermistor Deep Dive

### F2.1 — What is β (Beta)?

β (beta) describes how much the resistance changes with temperature. Higher β = more sensitive. CoreV2 uses β=3950, which is common for 10kΩ NTC thermistors.

Common β values: 3435, 3950, 4100, 4250. Using the wrong β in your calculation gives wrong temperatures.

### F2.2 — Steinhart-Hart vs Simplified B-Parameter Equation

```cpp
// Simplified B-parameter (what we use — good enough for ±0.5°C):
float tempK = 1.0f / (1.0f/T0 + log(R/R0)/B);

// Full Steinhart-Hart (more accurate, ±0.01°C):
// 1/T = A + B*ln(R) + C*ln(R)^3
// Requires 3 calibration points to determine A, B, C
// Overkill for plant monitoring
```

### F2.3 — Self-Heating Error

When you pass current through a thermistor to measure it, the current heats the thermistor slightly, causing it to read higher than actual temperature. This is self-heating error.

```
Power dissipated in thermistor: P = I²R = V²/R
At 3.3V with 10kΩ thermistor + 10kΩ R6:
  Current through divider: I = 3.3V / 20kΩ = 165µA
  Power in thermistor: P = (165µA)² × 10kΩ = 0.27µW
  Self-heating: ~0.001°C — NEGLIGIBLE ✅

If you read every 10ms continuously:
  Average power increases slightly but still negligible
```

### F2.4 — Common Thermistor Failures

| Failure Mode | Symptom | Cause |
|-------------|---------|-------|
| Open circuit | Reads -273°C (or max cold) | Thermistor broken, wire disconnected |
| Short circuit | Reads max hot | Thermistor shorted, solder bridge |
| Drift | Temperature slowly rises over hours | Self-heating from too-frequent readings, or nearby heat source |
| Mechanical damage | Intermittent readings | Glass bead cracked. Replace thermistor |

---

## F3. PIR Motion Sensor Deep Dive

### F3.1 — How PIR Actually Works

PIR (Passive InfraRed) sensors detect changes in infrared radiation. Humans emit IR at ~9-10μm wavelength. The sensor has two slots — when a warm body moves from one slot's field of view to the other, the differential signal triggers.

The Fresnel lens in front focuses IR from different zones onto the sensor element, creating a pattern of "hot" and "cold" zones. As a person walks through these zones, the sensor sees alternating IR levels.

### F3.2 — PIR Blind Spots

PIR sensors detect **motion across** their field of view, not **motion toward/away**. Walking directly toward a PIR sensor may not trigger it. Walking across its field of view WILL trigger it.

```
     PIR Sensor
        │
   ┌────┼────┐  ← Detection zones (Fresnel lens pattern)
   │    │    │
───┼────┼────┼───  Walking ACROSS = detected ✅
→  │    │    │
───┼────┼────┼───
   │    │    │
                  Walking TOWARD = may miss ❌
                       ↑
```

### F3.3 — PIR False Trigger Sources

| Source | Why It Triggers | Mitigation |
|--------|----------------|------------|
| Direct sunlight | Massive IR source, saturates sensor | Shade sensor, point away from windows |
| HVAC vents | Moving hot/cold air masses | Point sensor away from vents |
| Curtains moving | Sun-heated curtain moves in breeze | Secure curtains, or point sensor away |
| Small animals | Cats, dogs, rodents emit IR too | Use "pet immune" PIR or elevate sensor |
| Electrical interference | Motors, relays, fluorescent lights | Keep sensor wires away from power cables |
| Rapid temperature change | Sensor acclimating to environment | Wait 60 seconds after power-on |

---

## F4. IR Communication Deep Dive

### F4.1 — Why 38kHz?

IR receivers use a 38kHz carrier to filter out ambient IR. The receiver only responds to IR modulated at exactly 38kHz. This is why TV remotes work in sunlight — the receiver ignores the sun's constant IR and only sees the 38kHz pulses.

```cpp
// IRremote handles the 38kHz modulation automatically.
// All you need to do is send the data:
IrSender.sendNEC(0x00EF, 0x34, 0);  // address, command, repeats

// The library modulates this onto a 38kHz carrier for you.
```

### F4.2 — NEC Protocol Basics

Most IR remotes use the NEC protocol:

```
Start:  9ms HIGH, 4.5ms LOW
Logic 0: 562.5µs HIGH, 562.5µs LOW
Logic 1: 562.5µs HIGH, 1687.5µs LOW
Stop:   562.5µs HIGH

Total: 32 data bits + start/stop = ~67.5ms per transmission
```

### F4.3 — IR Range Factors

| Factor | Effect on Range |
|--------|----------------|
| LED current | Higher current = more IR output = longer range. 100Ω resistor = ~20mA through LED |
| LED beam angle | Narrow beam (20°) = longer range. Wide beam (60°) = shorter range but wider coverage |
| Receiver sensitivity | TSOP38238 is standard. TSOP31238 is higher sensitivity |
| Ambient light | Direct sunlight reduces range by 80%+. Fluorescent lights by 30% |
| Alignment | IR is line-of-sight. 10° misalignment at 2m = beam misses entirely |

---

## F5. Analog Joystick Deep Dive

### F5.1 — How the Joystick Works

The CoreV2 joystick uses two potentiometers (X and Y axis) arranged as voltage dividers. Moving the stick changes the wiper position, which changes the output voltage.

```
     +3.3V
       │
       ├── 10kΩ Pot (X axis)
       │      │
       │      ├── Wiper → GPIO4 (or GPIO7)
       │      │
       │     GND
       │
       ├── 10kΩ Pot (Y axis)
       │      │
       │      ├── Wiper → GPIO5 (or GPIO6)
       │      │
       │     GND
       │
       └── Switch → GPIO15 (active LOW, with pull-up to 3.3V)
```

### F5.2 — Why Center Isn't 2048

A perfect potentiometer would read ~2048 at center (half of 4095). Real joysticks have:

- **Mechanical offset:** The stick's physical center may not align with potentiometer center
- **Potentiometer tolerance:** ±20% resistance variation
- **ADC nonlinearity:** ESP32 ADC is not perfectly linear

CoreV2 measured values (Doom calibration):
```
X center: 1885  (NOT 2048!)
Y center: 1918  (NOT 2048!)
```

**Always calibrate for each individual board.** The values above are a starting point.

### F5.3 — Joystick Deadzone

Without a deadzone, the character/pointer will drift because the joystick never returns to exactly the same center position after release.

```cpp
float applyDeadzone(float value, float deadzone) {
  if (abs(value) < deadzone) return 0.0f;
  // Rescale so that deadzone→1.0 maps smoothly
  float sign = (value > 0) ? 1.0f : -1.0f;
  return sign * (abs(value) - deadzone) / (1.0f - deadzone);
}
```

### F5.4 — Joystick Click Button

The joystick click (GPIO15) is a simple momentary switch to GND with an internal pull-up. It's **active LOW**:

```cpp
bool joystickClicked() {
  return !digitalRead(15);  // LOW = pressed = true
}
```

No debounce needed for click (it's a single mechanical event). But debounce is still good practice for reliability.

---

---

## Quick Reference: Emergency Diagnostic Commands

### Run These When ANYTHING Is Broken

```cpp
// ═══ EMERGENCY DIAGNOSTIC SKETCH ═══
// Upload this to a misbehaving CoreV2 to isolate the problem

#include <Arduino.h>
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== COREV2 EMERGENCY DIAGNOSTIC ===");

  // 1. Chip identity
  Serial.printf("Chip:    %s rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("Cores:   %d @ %d MHz\n", ESP.getChipCores(), ESP.getCpuFreqMHz());
  Serial.printf("Flash:   %d MB\n", ESP.getFlashChipSize() / 1048576);
  Serial.printf("PSRAM:   %d MB\n", ESP.getPsramSize() / 1048576);
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());

  // 2. I2C scan
  Serial.println("\n--- I2C Scan (SDA=8, SCL=9) ---");
  Wire.begin(8, 9);
  int found = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  0x%02X", addr);
      if (addr == 0x3C) Serial.print(" (OLED)");
      if (addr == 0x3D) Serial.print(" (OLED alt)");
      if (addr == 0x20) Serial.print(" (PCF8574)");
      Serial.println();
      found++;
    }
  }
  if (found == 0) Serial.println("  ❌ NO I2C DEVICES!");

  // 3. PSRAM test
  if (ESP.getPsramSize() > 0) {
    void* ptr = heap_caps_malloc(1024, MALLOC_CAP_SPIRAM);
    if (ptr) {
      Serial.println("✅ PSRAM: OK (allocated 1KB)");
      free(ptr);
    } else {
      Serial.println("❌ PSRAM: FAILED to allocate!");
    }
  } else {
    Serial.println("⚠️  PSRAM: Not enabled in Tools menu!");
  }

  // 4. GPIO safety check
  Serial.println("\n--- GPIO Safety ---");
  for (int p = 22; p <= 37; p++) {
    Serial.printf("  GPIO%d: ⛔ RESERVED (PSRAM) — DO NOT USE\n", p);
  }

  // 5. Voltage check
  Serial.println("\n--- Voltage ---");
  int raw = analogRead(1);  // Any ADC pin
  float v = (raw / 4095.0f) * 3.3f;
  Serial.printf("  ADC test: raw=%d, voltage=%.2fV (expect ~1.5-2.0V from thermistor divider)\n", raw, v);

  Serial.println("\n=== DIAGNOSTIC COMPLETE ===");
}

void loop() {
  delay(1000);
}
```

---

> **Document Version:** 1.0 — 2026-07-14  
> **Coverage:** Every conceivable failure mode across all 4 summer camp projects  
> **How to use this guide:** Start with the project-specific section (B/C/D/E), then check Universal (A) if the problem spans multiple domains, then deep-dive into the sensor-specific section (F) if needed.  
> **When all else fails:** Run the Emergency Diagnostic Sketch above. Post the Serial output. It answers 80% of questions instantly.
