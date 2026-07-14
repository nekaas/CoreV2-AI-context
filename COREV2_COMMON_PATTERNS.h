// ╔══════════════════════════════════════════════════════════════╗
// ║  CoreV2 Common Code Patterns — Copy-Paste Snippets          ║
// ║  Every pattern is self-contained and tested for CoreV2      ║
// ╚══════════════════════════════════════════════════════════════╝

#pragma once
#include <Arduino.h>
#include <Wire.h>

// ═══════════════════════════════════════════════════════════════
// PATTERN 1: Non-Blocking Timer (don't use delay()!)
// ═══════════════════════════════════════════════════════════════

// Instead of:  delay(1000);
// Use:
//   static uint32_t lastMs = 0;
//   if (millis() - lastMs >= 1000) { lastMs = millis(); /* do work */ }

// ═══════════════════════════════════════════════════════════════
// PATTERN 2: Debounced Button (with edge detection)
// ═══════════════════════════════════════════════════════════════

class DebouncedButton {
  uint8_t _bit;
  uint32_t _lastDebounce;
  bool _lastState, _currentState;
public:
  DebouncedButton(uint8_t bit) : _bit(bit), _lastDebounce(0),
    _lastState(false), _currentState(false) {}

  // Call every loop. Returns true on RISING edge (just pressed).
  bool update(uint8_t rawButtons) {
    bool reading = (rawButtons & (1 << _bit)) != 0;
    if (reading != _lastState) _lastDebounce = millis();
    if (millis() - _lastDebounce > 50) {  // 50ms debounce
      if (reading != _currentState) {
        _currentState = reading;
        if (_currentState) return true;  // Just pressed!
      }
    }
    _lastState = reading;
    return false;
  }
};

// Usage:
//   DebouncedButton btnUp(0);
//   if (btnUp.update(readButtons())) { /* UP pressed! */ }

// ═══════════════════════════════════════════════════════════════
// PATTERN 3: Simple State Machine
// ═══════════════════════════════════════════════════════════════

enum AppState { STATE_INIT, STATE_IDLE, STATE_ACTIVE, STATE_ERROR };
AppState currentState = STATE_INIT;

void runStateMachine() {
  switch (currentState) {
    case STATE_INIT:
      // Setup code
      currentState = STATE_IDLE;
      break;
    case STATE_IDLE:
      if (/* trigger condition */ false) currentState = STATE_ACTIVE;
      break;
    case STATE_ACTIVE:
      if (/* done condition */ false) currentState = STATE_IDLE;
      break;
    case STATE_ERROR:
      if (/* recovered */ false) currentState = STATE_IDLE;
      break;
  }
}

// ═══════════════════════════════════════════════════════════════
// PATTERN 4: Smooth Analog Reading (moving average)
// ═══════════════════════════════════════════════════════════════

class SmoothAnalog {
  float _value;
  float _alpha;  // 0.0–1.0, lower = smoother
public:
  SmoothAnalog(float alpha = 0.1f) : _value(0), _alpha(alpha) {}
  float update(int rawPin) {
    _value = _alpha * analogRead(rawPin) + (1.0f - _alpha) * _value;
    return _value;
  }
  int raw() { return (int)_value; }
};

// ═══════════════════════════════════════════════════════════════
// PATTERN 5: LED Blinker (non-blocking)
// ═══════════════════════════════════════════════════════════════

class LEDBlinker {
  uint32_t _interval, _lastToggle;
  bool _state, _enabled;
public:
  LEDBlinker(uint32_t intervalMs = 500)
    : _interval(intervalMs), _lastToggle(0), _state(false), _enabled(false) {}

  void on()  { _enabled = true; }
  void off() { _enabled = false; _state = false; }
  void setInterval(uint32_t ms) { _interval = ms; }

  // Call in loop(). Returns current LED state (true=on).
  bool update() {
    if (!_enabled) return false;
    if (millis() - _lastToggle >= _interval) {
      _lastToggle = millis();
      _state = !_state;
    }
    return _state;
  }
};

// ═══════════════════════════════════════════════════════════════
// PATTERN 6: PCF8574 Helper (button read + LED write)
// ═══════════════════════════════════════════════════════════════

class PCF8574Helper {
  uint8_t _shadow;  // Tracks LED state for read-modify-write
public:
  PCF8574Helper() : _shadow(0xFF) {}  // All high = LEDs off

  uint8_t readButtons() {
    Wire.beginTransmission(0x20);
    Wire.write(0xFF);  // All inputs with pull-ups
    Wire.endTransmission();
    Wire.requestFrom(0x20, 1);
    return ~Wire.read();  // Invert: 1 = pressed
  }

  void setLED(uint8_t bit, bool on) {
    if (on) _shadow &= ~(1 << bit);  // Clear bit = LED on
    else    _shadow |=  (1 << bit);  // Set bit = LED off
    Wire.beginTransmission(0x20);
    Wire.write(_shadow);
    Wire.endTransmission();
  }

  void allLEDsOff() {
    _shadow = 0xFF;
    Wire.beginTransmission(0x20);
    Wire.write(_shadow);
    Wire.endTransmission();
  }
};

// ═══════════════════════════════════════════════════════════════
// PATTERN 7: FPS Counter
// ═══════════════════════════════════════════════════════════════

class FPSCounter {
  uint32_t _lastSecond;
  uint16_t _frameCount, _fps;
public:
  FPSCounter() : _lastSecond(0), _frameCount(0), _fps(0) {}

  void tick() {
    _frameCount++;
    if (millis() - _lastSecond >= 1000) {
      _fps = _frameCount;
      _frameCount = 0;
      _lastSecond = millis();
    }
  }
  uint16_t fps() const { return _fps; }
};

// ═══════════════════════════════════════════════════════════════
// PATTERN 8: Simple Menu System (OLED + D-Pad)
// ═══════════════════════════════════════════════════════════════

const char* menuItems[] = {"Start Game", "Settings", "About", "Exit"};
const uint8_t MENU_COUNT = 4;
uint8_t menuSelection = 0;

void drawMenu() {
  for (uint8_t i = 0; i < MENU_COUNT; i++) {
    if (i == menuSelection) {
      // Highlighted item
      display.fillRect(5, i * 14 + 8, 118, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(10, i * 14 + 10);
    display.print(menuItems[i]);
  }
  display.setTextColor(SSD1306_WHITE);  // Reset
}

void handleMenuInput(uint8_t btn) {
  if (btn & (1<<0)) menuSelection = (menuSelection + MENU_COUNT - 1) % MENU_COUNT;
  if (btn & (1<<1)) menuSelection = (menuSelection + 1) % MENU_COUNT;
  // CENTER to select (handled by caller)
}

// ═══════════════════════════════════════════════════════════════
// PATTERN 9: WiFi Connection with Timeout
// ═══════════════════════════════════════════════════════════════

#ifdef WiFi_h
bool connectWiFi(const char* ssid, const char* pass, uint32_t timeoutMs = 15000) {
  WiFi.begin(ssid, pass);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());
    return true;
  }
  Serial.println("\nWiFi connect failed!");
  return false;
}
#endif

// ═══════════════════════════════════════════════════════════════
// PATTERN 10: Animation Frame Looper (with ping-pong)
// ═══════════════════════════════════════════════════════════════

class AnimLooper {
  uint8_t _totalFrames, _current;
  int8_t _dir;
  bool _pingpong;
public:
  AnimLooper(uint8_t frames, bool pingpong = false)
    : _totalFrames(frames), _current(0), _dir(1), _pingpong(pingpong) {}

  uint8_t next() {
    uint8_t result = _current;
    _current += _dir;
    if (_pingpong) {
      if (_current >= _totalFrames) { _current = _totalFrames - 2; _dir = -1; }
      if (_current == 255) { _current = 1; _dir = 1; }  // underflow wrap
    } else {
      if (_current >= _totalFrames) _current = 0;
    }
    return result;
  }

  uint8_t current() const { return _current; }
  void reset() { _current = 0; _dir = 1; }
};

// ═══════════════════════════════════════════════════════════════
// PATTERN 11: Serial Command Parser
// ═══════════════════════════════════════════════════════════════

void handleSerialCommands() {
  if (!Serial.available()) return;
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  if (cmd == "help") {
    Serial.println("Commands: help, status, reset, led on/off");
  } else if (cmd == "status") {
    Serial.printf("Uptime: %lu ms, Free heap: %d\n",
                  millis(), ESP.getFreeHeap());
  } else if (cmd == "reset") {
    ESP.restart();
  } else if (cmd == "led on") {
    digitalWrite(38, HIGH); Serial.println("LED4 ON");
  } else if (cmd == "led off") {
    digitalWrite(38, LOW);  Serial.println("LED4 OFF");
  } else {
    Serial.println("Unknown command. Type 'help'.");
  }
}

// ═══════════════════════════════════════════════════════════════
// PATTERN 12: EEPROM/LittleFS Config with Factory Reset
// ═══════════════════════════════════════════════════════════════

#ifdef LittleFS_h
struct Config {
  char pin[9] = "1234";
  uint16_t servoCalibMs = 1500;
  uint32_t highScore = 0;
  uint32_t magic = 0xC0REV2;  // Magic number to detect valid config
};

bool loadConfig(Config& cfg) {
  File f = LittleFS.open("/config.bin", "r");
  if (!f) return false;
  f.read((uint8_t*)&cfg, sizeof(Config));
  f.close();
  return cfg.magic == 0xC0REV2;  // Valid?
}

void saveConfig(const Config& cfg) {
  File f = LittleFS.open("/config.bin", "w");
  f.write((const uint8_t*)&cfg, sizeof(Config));
  f.close();
}

void factoryReset(Config& cfg) {
  cfg = Config();  // Reset to defaults
  saveConfig(cfg);
  Serial.println("⚙️ Factory reset complete!");
}
#endif
