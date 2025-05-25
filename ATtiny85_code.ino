#include <SPI.h>

#define PWM_2HZ_PIN     0  // PB0 (Pin 5) - Hardware PWM
#define PWM_4KHZ_PIN    1  // PB1 (Pin 6) - Software PWM
#define MCP_SCK_PIN     2  // PB2 (Pin 7)
#define MCP_SI_PIN      3  // PB3 (Pin 2)
#define MCP_CS_PIN      4  // PB4 (Pin 3)

// MCP4251 commands
#define MCP_WRITE       0x00
#define MCP_WIPER0      0x00
#define MCP_WIPER1      0x10

const uint16_t PWM_4KHZ_HALF_PERIOD = 125 / 16; // 4kHz software PWM (µs)
volatile uint32_t lastToggleTime = 0;

void setup() {
  // --- Hardware 2Hz PWM on PB0 using Timer0 ---
  pinMode(PWM_2HZ_PIN, OUTPUT);
  TCCR0A = _BV(COM0A0);
  TCCR0B = _BV(WGM02) | _BV(CS02) | _BV(CS00); // CTC mode, prescaler 1024
  OCR0A = 244;  // 8MHz / 1024 / 3906 ≈ 2Hz

  // --- Software 4kHz PWM on PB1 ---
  pinMode(PWM_4KHZ_PIN, OUTPUT);
  digitalWrite(PWM_4KHZ_PIN, LOW);

  // --- SPI setup for MCP4251 ---
  pinMode(MCP_SCK_PIN, OUTPUT);
  pinMode(MCP_SI_PIN, OUTPUT);
  pinMode(MCP_CS_PIN, OUTPUT);
  digitalWrite(MCP_CS_PIN, HIGH);
}

void loop() {
  // --- Software PWM 4kHz (non-blocking toggle) ---
  uint32_t currentMicros = micros();
  if (currentMicros - lastToggleTime >= PWM_4KHZ_HALF_PERIOD) {
    digitalWrite(PWM_4KHZ_PIN, !digitalRead(PWM_4KHZ_PIN));
    lastToggleTime = currentMicros;
  }

  // --- Linear ramp from 100Ω to 10kΩ over real 10s ---
  static uint8_t step = 0;
  static uint32_t lastUpdate = 0;

  if (step <= 128 && millis() - lastUpdate >= 5) { // ~10s total over real time
    float resistance = 100.0 + ((10000.0 - 100.0) * step / 128.0);
    uint8_t wiperValue = constrain(128 - step, 0, 128); // direct linear mapping
    setWiper(MCP_WIPER0, wiperValue);
    setWiper(MCP_WIPER1, wiperValue);

    step++;
    lastUpdate = millis();
  }
}

// --- Helper functions ---
void setWiper(uint8_t wiper, uint8_t value) {
  digitalWrite(MCP_CS_PIN, LOW);
  SPI_transfer(wiper | MCP_WRITE);
  SPI_transfer(value);
  digitalWrite(MCP_CS_PIN, HIGH);
}

void SPI_transfer(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(MCP_SI_PIN, data & 0x80);
    digitalWrite(MCP_SCK_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(MCP_SCK_PIN, LOW);
    delayMicroseconds(1);
    data <<= 1;
  }
}
