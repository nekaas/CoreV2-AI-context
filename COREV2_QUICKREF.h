// ╔══════════════════════════════════════════════════════════════╗
// ║  CoreV2 Quick Reference Card — 1 Page Pin Map               ║
// ║  🇱🇻 Izdrukā un pielīmē pie monitora.                        ║
// ║  SDA=8, SCL=9, OLED=0x3C, PCF8574=0x20                     ║
// ║  ⛔ GPIO22-37 = AIZLIEGTS (PSRAM)!                           ║
// ╚══════════════════════════════════════════════════════════════╝

#pragma once

// ═══════════════ MCU ═══════════════
// ESP32-S3-WROOM-1 N16R8 (Plate / Board)
// Dual Xtensa LX7 @ 240MHz, 16MB Flash, 8MB Octal PSRAM

// ═══════════════ I2C (GPIO8=SDA, GPIO9=SCL) ═══════════════
#define I2C_SDA       8     // Datu līnija
#define I2C_SCL       9     // Pulksteņa līnija
#define OLED_ADDR     0x3C  // SSD1306/SH1106 128x64 ekrāns
#define OLED_ALT      0x3D  // Alternatīvā adrese
#define PCF8574_ADDR  0x20     // 5 buttons + 3 LEDs

// ═══════════════ BUTTONS (PCF8574, active LOW) ═══════════════
#define BTN_UP        0   // P0 = SW1
#define BTN_DOWN      1   // P1 = SW3
#define BTN_LEFT      2   // P2 = SW5
#define BTN_RIGHT     3   // P3 = SW4
#define BTN_CENTER    7   // P7 = SW2

// ═══════════════ LEDs (PCF8574 P4/P5/P6, active HIGH) ═══════
#define LED1          4   // P4
#define LED3          5   // P5
#define LED2          6   // P6
#define LED4          38  // GPIO38 (direct, shared w/ IR TX)

// ═══════════════ ANALOG INPUTS ═══════════════
#define THERMISTOR    1   // NTC 10kΩ β=3950
#define MOISTURE      2   // Capacitive soil sensor (expansion)
#define JOY_X         4   // Joystick X (or 7)
#define JOY_Y         5   // Joystick Y (or 6)
#define JOY_CLICK     15  // Active LOW, INPUT_PULLUP

// ═══════════════ AUDIO ═══════════════
#define BUZZER_PIN    10  // GPIO10→D6→Q2→BZ1 (transistor driven!)
#define BUZZER_CH     1   // LEDC channel
#define I2S_BCLK      16  // PCM5102A
#define I2S_LRCLK     17
#define I2S_DIN       18

// ═══════════════ SERVO ═══════════════
#define SERVO_PIN     39  // Continuous rotation, 50Hz PWM
#define SERVO_CH      0   // LEDC channel
// STOP=307, CW=348, CCW=266 (12-bit duty, 50Hz)

// ═══════════════ IR ═══════════════
#define IR_RX         40
#define IR_TX         38  // Kopīgs ar LED4

// ═══════════════ SD CARD (SPI) ═══════════════
#define SD_MOSI       11  // Dati uz karti
#define SD_MISO       13  // Dati no kartes
#define SD_CLK        12  // Pulkstenis
#define SD_CS         14  // Chip Select
#define SD_CD         21  // Kartes detektors (LOW=ievietota)

// ═══════════════ USB ═══════════════
#define USB_DM        19  // USB D-
#define USB_DP        20  // USB D+

// ═══════════════ UART ═══════════════
#define UART_TX       43  // Serial1 TX (raidīt)
#define UART_RX       44  // Serial1 RX (saņemt)

// ═══════════════ EXPANSION (Paplašinājumi) ═══════════════
#define EXP_GPIO2     2   // Piem: mitruma sensors
#define EXP_GPIO39    39  // Piem: servo (kopīgs!)
#define EXP_GPIO41    41  // Piem: PIR sensors
#define EXP_GPIO42    42  // Piem: status LED / sirēna

// ═══════════════ ENCODER (Rotācijas kodētājs) ═══════════════
#define ENC_A         4   // Konfliktē ar JOY_X!
#define ENC_B         5   // Konfliktē ar JOY_Y!

// ═══════════════ SERVO PWM ═══════════════
#define SERVO_CH      0   // LEDC kanāls 0
#define SERVO_FREQ    50  // 50 Hz standarta servo
#define SERVO_RES     12  // 12-bitu izšķirtspēja
#define SERVO_STOP    307 // 1.5ms = STOP (duty pie 12-bit/50Hz)
#define SERVO_CW      348 // 1.7ms = griežas pulksteņrād. virzienā
#define SERVO_CCW     266 // 1.3ms = griežas pretēji
#define SERVO_90DEG_MS 1500 // ~90° kalibrējamais laiks (ms)

// ═══════════════ AUDIO (Skaņa) ═══════════════
#define BUZZER_CH     1   // LEDC kanāls 1 (GPIO10→D6→Q2→BZ1)
#define BUZZER_FREQ   440 // Bāzes frekvence (A4 nots)
#define BUZZER_RES    10  // 10-bitu izšķirtspēja
#define I2S_BCLK      16  // PCM5102A DAC
#define I2S_LRCLK     17
#define I2S_DIN       18

// ═══════════════ ⛔ AIZLIEGTS (NEKAD NEIZMANTO) ⛔ ═══════════════
// GPIO22-37 = Octal PSRAM atmiņas līnijas. Izmantošana = AVĀRIJA.
// GPIO0, GPIO3, GPIO45, GPIO46 = Strapping pini (boot konfigurācija).
// ⚠️  NOPIETNI: GPIO22-37 ir rezervēti. Plate uzkārsies acumirklī.

// ═══════════════ Biežāk lietotie I²C skenēšanas rezultāti ═══════════════
// Pareiza I²C kopne:
//   ✅ 0x20 — PCF8574 (pogas + LED)
//   ✅ 0x3C — SSD1306/SH1106 OLED ekrāns
// Ja kaut kā trūkst → pārbaudi vadus un barošanu!

// ═══════════════ Biežāk lietotās komandas ═══════════════
// Wire.begin(8, 9);                  — I²C inicializācija
// display.begin(SWITCHCAPVCC,0x3C);  — OLED inicializācija
// ledcSetup(0, 50, 12);             — Servo PWM iestatīšana
// ledcAttachPin(39, 0);             — Servo pieslēgšana GPIO39
// ledcSetup(1, 440, 10);            — Zummers PWM iestatīšana
// ledcAttachPin(10, 1);             — Zummers pieslēgšana GPIO10
// SPI.begin(12, 13, 11);            — SD kartes SPI (CLK,MISO,MOSI)
// SD.begin(14);                     — SD kartes inicializācija
// LittleFS.begin(true);             — Failu sistēmas inicializācija
