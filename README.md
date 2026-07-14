# CoreV2 AI Context — Complete Hardware & Firmware Reference

> **Single source of truth for the CoreV2 electronics platform**  
> ESP32-S3 N16R8 · 128×64 OLED (SSD1306/SH1106) · 5-Button D-Pad (PCF8574) · 360° Servo · IR TX/RX · Buzzer · WiFi · BLE  
> **Environment:** Electronics Summer Camp — 4 student projects in 🇱🇻 Latvian

---

## 📦 What This Repository Contains

This repository is the **complete AI context window** for the CoreV2 hardware platform. It provides everything an AI assistant (and instructor) needs to help students build, debug, and extend their projects.

| File | Lines | Purpose |
|------|-------|---------|
| [`COREV2_PCB_FULL_CONTEXT.md`](./COREV2_PCB_FULL_CONTEXT.md) | 1,607 | Complete hardware reference: pinout, I²C devices, peripherals, power management |
| [`COREV2_QUICKREF.h`](./COREV2_QUICKREF.h) | 119 | 1-page pin map — print & stick to your monitor |
| [`COREV2_COMMON_PATTERNS.h`](./COREV2_COMMON_PATTERNS.h) | 305 | Reusable code patterns: debouncing, state machines, non-blocking timers, smoothing |
| [`COREV2_STARTER_TEMPLATES.h`](./COREV2_STARTER_TEMPLATES.h) | 335 | Copy-paste project skeletons for all 4 camp projects |
| [`COREV2_ANIMATION_LIBRARY.h`](./COREV2_ANIMATION_LIBRARY.h) | 958 | Complete OLED icon & animation catalog (32×32 PROGMEM bitmaps) |
| [`COREV2_OLED_ANIMATION_GUIDE.md`](./COREV2_OLED_ANIMATION_GUIDE.md) | 1,109 | Wokwi Animator workflow, sprite sheets, procedural animations |
| [`COREV2_TROUBLESHOOTING_GUIDE.md`](./COREV2_TROUBLESHOOTING_GUIDE.md) | 1,950 | Every failure mode, fix time, and 🇱🇻 Latvian error translation index |

---

## 🎯 Camp Projects

| Project | Core Skill | Key Hardware |
|---------|-----------|-------------|
| 🎮 **Game Boy** | Game loops, sprites, D-Pad, sound FX | OLED, PCF8574 buttons, buzzer, SD card |
| 🌱 **Smart Plant Pot** | ADC sensors, calibration, monitoring | Moisture sensor, NTC thermistor, OLED |
| 🔒 **Smart Safe** | Servo PWM, PIN auth, state machines | 360° servo, PCF8574, LittleFS storage |
| 🚨 **Security System** | IR motion, ESP-NOW mesh, logging | PIR, IR TX/RX, ESP32-C3 Supermini, SD card |

---

## 🚀 Quick Start

### For AI Assistants

Read the **AI First Response Protocol** in [`COREV2_PCB_FULL_CONTEXT.md`](./COREV2_PCB_FULL_CONTEXT.md) before responding to any CoreV2 request. The protocol covers:

1. **Request classification** — which skill to invoke
2. **Mandatory code requirements** — every sketch must include correct pins, Serial, Wire
3. **Safety checklist** — PSRAM pins (GPIO22-37) are forbidden, servo needs 5V
4. **Latvian translation table** — what students actually mean when they describe a problem

### For Students & Instructors

1. **Pin lookup** → [`COREV2_QUICKREF.h`](./COREV2_QUICKREF.h) — print this!
2. **Starting a new project** → [`COREV2_STARTER_TEMPLATES.h`](./COREV2_STARTER_TEMPLATES.h) — copy the skeleton
3. **Adding icons/animation** → [`COREV2_ANIMATION_LIBRARY.h`](./COREV2_ANIMATION_LIBRARY.h) — pre-built catalog
4. **Something broken?** → [`COREV2_TROUBLESHOOTING_GUIDE.md`](./COREV2_TROUBLESHOOTING_GUIDE.md) — find your error in Latvian

---

## 🔌 Hardware at a Glance

```
ESP32-S3-WROOM-1 N16R8
├── I²C Bus (SDA=8, SCL=9)
│   ├── SSD1306/SH1106 OLED 128×64 @ 0x3C
│   └── PCF8574 I/O Expander @ 0x20
│       ├── P0-P3, P7: D-Pad buttons (UP, DOWN, LEFT, RIGHT, CENTER)
│       └── P4-P6: LEDs
├── GPIO10 → Buzzer (transistor-driven)
├── GPIO39 → 360° Continuous Rotation Servo
├── GPIO38 → LED4 / IR TX (shared)
├── ADC1: Thermistor (NTC 10kΩ β=3950)
├── ADC2: Capacitive Soil Moisture
├── ADC4/5: Analog Joystick (X/Y)
├── GPIO15: Joystick Click
├── IR Receiver (onboard)
└── microSD Card Slot (SPI)
```

> ⛔ **CRITICAL:** GPIO22-37 are reserved for Octal PSRAM. NEVER use these pins!

---

## 📚 Companion Skills

This repository works with the CoreV2 skill pack (12 skills):

| Skill | Domain |
|-------|--------|
| `corev2-pcb` | Hardware pinout, I²C addresses, power |
| `corev2-oled-animations` | OLED display, icons, sprite rendering |
| `corev2-gameboy` | Retro gaming: game loops, sprites, sound |
| `corev2-plant-pot` | Moisture sensing, thermistor, plant dashboard |
| `corev2-smart-safe` | Servo lock, PIN entry, LittleFS config |
| `corev2-security-system` | PIR/IR sensors, ESP-NOW mesh, SD logging |
| `corev2-debugging` | I²C scanner, test sketches, diagnosis |
| `corev2-wifi-web` | WiFi AP/STA, web servers, captive portal |
| `corev2-audio` | Buzzer melodies, note tables, I²S DAC |
| `corev2-storage` | LittleFS, SD card, config persistence |
| `corev2-sensors` | ADC, thermistor, moisture, IR, joystick, PIR |
| `corev2-camp-teaching` | Teaching strategies, common student mistakes |

---

## 🇱🇻 Language Note

All camp students speak **Latvian only**. All AI responses, Serial Monitor messages, error explanations, and code comments in student-facing code should be in Latvian. See the Latvian technical glossary in [`COREV2_PCB_FULL_CONTEXT.md`](./COREV2_PCB_FULL_CONTEXT.md).

---

## 📄 License

Internal educational use — CoreV2 Electronics Summer Camp.
