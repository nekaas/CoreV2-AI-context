// ╔══════════════════════════════════════════════════════════════╗
// ║  CoreV2 Starter Templates — Copy-Paste Project Skeletons    ║
// ║  Each template compiles out of the box (just fill in TODO)  ║
// ╚══════════════════════════════════════════════════════════════╝

// ═══════════════════════════════════════════════════════════════
// TEMPLATE 1: Game Boy — Minimal Game Loop
// ═══════════════════════════════════════════════════════════════

#ifdef BUILD_GAMEBOY
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_W 128
#define SCREEN_H 64
Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

// ── Game State ───────────────────────────────────
enum State { TITLE, PLAYING, PAUSED, OVER };
State state = TITLE;

// ── Button Reading ───────────────────────────────
uint8_t readButtons() {
  Wire.beginTransmission(0x20); Wire.write(0xFF);
  Wire.endTransmission(); Wire.requestFrom(0x20, 1);
  return ~Wire.read();
}
#define BTN(pressed, bit) ((pressed) & (1 << (bit)))

// ── Buzzer ───────────────────────────────────────
void beep(int freq, int dur) {
  ledcWriteTone(1, freq); delay(dur); ledcWriteTone(1, 0);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ledcSetup(1, 440, 10); ledcAttachPin(10, 1);
  display.clearDisplay(); display.display();
  Serial.println("🎮 Game Boy ready!");
}

void loop() {
  uint8_t btn = readButtons();

  display.clearDisplay();

  switch (state) {
    case TITLE:
      display.setCursor(20, 20); display.print("MY GAME");
      display.setCursor(15, 40); display.print("Press CENTER");
      if (BTN(btn, 7)) { state = PLAYING; beep(880, 50); }
      break;

    case PLAYING:
      // TODO: Your game logic here!
      display.setCursor(30, 28); display.print("GAME ON!");
      if (BTN(btn, 7)) { state = PAUSED; }
      break;

    case PAUSED:
      display.setCursor(35, 28); display.print("PAUSED");
      if (BTN(btn, 7)) { state = PLAYING; }
      break;

    case OVER:
      display.setCursor(25, 28); display.print("GAME OVER");
      if (BTN(btn, 7)) { state = TITLE; }
      break;
  }

  display.display();
  delay(30);  // ~30 FPS max
}
#endif

// ═══════════════════════════════════════════════════════════════
// TEMPLATE 2: Smart Plant Pot — Sensor Monitor
// ═══════════════════════════════════════════════════════════════

#ifdef BUILD_PLANTPOT
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ── Sensor Reading ──────────────────────────────
int readMoisture() {
  long sum = 0;
  for (int i = 0; i < 10; i++) { sum += analogRead(2); delay(5); }
  return sum / 10;
}

float readTemp() {
  int raw = analogRead(1);
  float v = (raw / 4095.0f) * 3.3f;
  float r = (3.3f * 10000.0f / v) - 10000.0f;
  float k = 1.0f / (1.0f/298.15f + log(r/10000.0f)/3950.0f);
  return k - 273.15f;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ledcSetup(1, 440, 10); ledcAttachPin(10, 1);
  Serial.println("🌱 Plant Pot ready!");
}

void loop() {
  int moist = readMoisture();
  float temp = readTemp();

  // Map moisture to percentage (calibrate these!)
  int pct = constrain(map(moist, 2000, 500, 0, 100), 0, 100);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);  display.print("🌱 My Plant");
  display.setCursor(0, 16); display.printf("Moisture: %d%%", pct);
  display.setCursor(0, 28); display.printf("Temp: %.1f C", temp);

  // Status line
  display.setCursor(0, 44);
  if (pct < 30) {
    display.print("🔴 Water me!");
    ledcWriteTone(1, 880); delay(200); ledcWriteTone(1, 0);
  } else if (pct < 60) {
    display.print("🟡 OK");
  } else {
    display.print("🟢 Happy!");
  }
  display.display();

  // Update PCF8574 LEDs based on moisture
  uint8_t ledBits = 0;
  if (pct > 60) ledBits |= (1<<4);       // Green
  else if (pct > 30) ledBits |= (1<<5);  // Yellow
  else ledBits |= (1<<6);                 // Red
  Wire.beginTransmission(0x20);
  Wire.write(0xFF & ~ledBits);  // Turn on selected LED
  Wire.endTransmission();

  delay(2000);  // Update every 2 seconds
}
#endif

// ═══════════════════════════════════════════════════════════════
// TEMPLATE 3: Smart Safe — Servo Lock + PIN Entry
// ═══════════════════════════════════════════════════════════════

#ifdef BUILD_SAFE
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ── Servo ───────────────────────────────────────
void servoStop()  { ledcWrite(0, 307); }
void servoLock()  { ledcWrite(0, 266); delay(1500); servoStop(); }
void servoUnlock(){ ledcWrite(0, 348); delay(1500); servoStop(); }

// ── Button Read ─────────────────────────────────
uint8_t readBtns() {
  Wire.beginTransmission(0x20); Wire.write(0xFF);
  Wire.endTransmission(); Wire.requestFrom(0x20, 1);
  return ~Wire.read();
}

// ── PIN State ───────────────────────────────────
uint8_t pin[4] = {1,2,3,4};  // Default PIN
uint8_t entry[4] = {0,0,0,0};
uint8_t cursor = 0;
bool locked = true;

void drawPINEntry() {
  display.clearDisplay();
  display.setCursor(10, 10); display.print("ENTER PIN:");
  display.setCursor(25, 35);
  for (int i = 0; i < 4; i++) {
    if (i == cursor) display.print("[");
    display.print(entry[i]);
    if (i == cursor) display.print("]");
    display.print(" ");
  }
  display.display();
}

bool checkPIN() {
  for (int i = 0; i < 4; i++)
    if (entry[i] != pin[i]) return false;
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ledcSetup(0, 50, 12); ledcAttachPin(39, 0);
  ledcSetup(1, 440, 10); ledcAttachPin(10, 1);
  servoStop();
  Serial.println("🔐 Smart Safe ready!");
}

void loop() {
  uint8_t btn = readBtns();

  if (locked) {
    drawPINEntry();

    if (btn & (1<<0)) entry[cursor] = (entry[cursor] + 1) % 10;     // UP
    if (btn & (1<<1)) entry[cursor] = (entry[cursor] + 9) % 10;     // DOWN
    if (btn & (1<<2)) cursor = (cursor + 3) % 4;                     // LEFT
    if (btn & (1<<3)) cursor = (cursor + 1) % 4;                     // RIGHT

    if (btn & (1<<7)) {  // CENTER = confirm
      if (checkPIN()) {
        locked = false;
        servoUnlock();
        ledcWriteTone(1, 1047); delay(500); ledcWriteTone(1, 0);  // Victory!
        Serial.println("✅ UNLOCKED!");
      } else {
        // Error buzz
        for (int i = 0; i < 3; i++) {
          ledcWriteTone(1, 220); delay(150);
          ledcWriteTone(1, 0); delay(100);
        }
        Serial.println("❌ Wrong PIN!");
        memset(entry, 0, sizeof(entry)); cursor = 0;
      }
    }

    delay(150);  // Debounce
  } else {
    display.clearDisplay();
    display.setCursor(20, 25); display.print("🔓 UNLOCKED");
    display.setCursor(10, 40); display.print("CENTER to lock");
    display.display();

    if (btn & (1<<7)) {
      locked = true;
      servoLock();
    }
    delay(150);
  }
}
#endif

// ═══════════════════════════════════════════════════════════════
// TEMPLATE 4: Security System — PIR + IR + Alarm
// ═══════════════════════════════════════════════════════════════

#ifdef BUILD_SECURITY
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

#define PIR_PIN   41
#define IR_RX_PIN 40
bool armed = false;
bool alarmActive = false;
uint32_t alarmStart = 0;

uint8_t readBtns() {
  Wire.beginTransmission(0x20); Wire.write(0xFF);
  Wire.endTransmission(); Wire.requestFrom(0x20, 1);
  return ~Wire.read();
}

void sirenOn()  { ledcWriteTone(1, 880); }
void sirenOff() { ledcWriteTone(1, 0); }

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ledcSetup(1, 440, 10); ledcAttachPin(10, 1);
  pinMode(PIR_PIN, INPUT);
  Serial.println("🚨 Security System ready!");
  Serial.println("PIR warming up... 30 seconds...");
  delay(30000);  // PIR warmup
}

void loop() {
  uint8_t btn = readBtns();

  // Arm/Disarm
  if (btn & (1<<0)) { armed = true;  Serial.println("🔒 ARMED"); }
  if (btn & (1<<1)) { armed = false; alarmActive = false; sirenOff();
                      Serial.println("🔓 DISARMED"); }

  // Motion detection (only when armed)
  if (armed && digitalRead(PIR_PIN) && !alarmActive) {
    alarmActive = true;
    alarmStart = millis();
    Serial.println("🚨 INTRUSION DETECTED!");
  }

  // Alarm timeout (5 seconds)
  if (alarmActive && millis() - alarmStart > 5000) {
    alarmActive = false;
    sirenOff();
  }

  // Siren pattern
  if (alarmActive) {
    uint32_t t = (millis() - alarmStart) % 600;
    if (t < 300) sirenOn(); else sirenOff();
  }

  // Display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(armed ? "🔒 ARMED" : "🔓 DISARMED");
  display.setCursor(0, 16);
  display.printf("PIR: %s", digitalRead(PIR_PIN) ? "MOTION" : "clear");
  display.setCursor(0, 28);
  display.printf("IR:  %s", "---");  // TODO: IR beam status
  display.setCursor(0, 44);
  if (alarmActive) display.print("🚨 ALARM ACTIVE!");
  display.display();

  delay(100);
}
#endif
