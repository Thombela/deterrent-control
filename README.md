# ATtiny85 Deterrent Subsystem Firmware

This repository contains the Arduino sketch used to program an ATtiny85 microcontroller for use in a dual-mode **animal deterrent subsystem**. The subsystem integrates both **audio** (piezoelectric buzzer) and **visual** (flashing LED) deterrents to mitigate animal interference through timed intensity scaling.

## 🔧 Features

- Generates two PWM signals:
  - 4 kHz for the buzzer (software PWM).
  - 2 Hz for the LED (hardware PWM).
- Gradually increases deterrent intensity over time using a digital potentiometer (MCP4251).
- Designed for low-power operation, activating only when triggered by a detection subsystem.
- Fully compatible with the ATtiny85-20PU running at 8 MHz internal clock.

---

## 🛠 Requirements

- Arduino Uno R3 (used as ISP programmer)
- ATtiny85-20PU
- Arduino IDE (1.8.x or 2.x)
- 10 µF capacitor
- Breadboard or programming jig
- Jumper wires
- **[Optional]** MCP4251 dual digital potentiometer

---

## 🧠 How It Works

- The sketch configures the ATtiny85 to output two PWM signals and communicates with a digital potentiometer via SPI.
- The buzzer frequency is ramped over time by increasing the resistance using the digital potentiometer.
- The LED is flashed using a hardware timer at a low frequency to save power.
- The ATtiny85 receives a signal from the detection subsystem to activate the deterrent.

---

## 🪛 How to Program the ATtiny85 with Arduino Uno

### 1. Prepare the Arduino Uno as an ISP

1. Open the **Arduino IDE**.
2. Go to `File` > `Examples` > `11.ArduinoISP` > `ArduinoISP`.
3. Select the Arduino Uno board under `Tools > Board`.
4. Select the correct COM port.
5. Upload the sketch.

### 2. Hardware Connections

| Arduino Uno Pin | ATtiny85 Pin | Description             |
|------------------|--------------|--------------------------|
| D10              | Pin 1        | RESET                    |
| D11              | Pin 5        | MOSI                     |
| D12              | Pin 6        | MISO                     |
| D13              | Pin 7        | SCK                      |
| 5V               | Pin 8        | VCC                      |
| GND              | Pin 4        | GND                      |

> ⚠️ **Important:** Place a 10 µF capacitor between the Uno’s RESET and GND pins to prevent auto-reset during programming.

### 3. Board Manager Configuration

1. Install ATtiny board support via the **Boards Manager**:
   - Add this URL in `Preferences > Additional Board URLs`:
     ```
     https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
     ```
2. Go to `Tools > Board > ATtiny25/45/85`.
3. Set the following:
   - **Processor:** ATtiny85
   - **Clock:** 8 MHz (Internal)
   - **Programmer:** Arduino as ISP

### 4. Upload the Sketch

- Use `Sketch > Upload Using Programmer` (⇧ + ⌘ + U / ⇧ + Ctrl + U) to upload the code to the ATtiny.

---

## 🧪 Troubleshooting

- If the buzzer or LED do not operate simultaneously, check your PWM allocation—hardware PWM is available on only one timer.
- If timings are inaccurate, verify that the ATtiny is set to **8 MHz internal clock** and has been burned via **Tools > Burn Bootloader** before uploading.

---

## 📁 Repository Contents

```bash
ATtiny_Deterrent.ino   # Main Arduino sketch for programming the ATtiny85
