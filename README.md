# ESP32 Telegram Climate Monitor

A non-blocking, asynchronous environmental monitoring system built on the standard ESP32 platform. The device tracks ambient temperature and humidity in real-time using a DHT11 sensor, displays live metrics locally on an SSD1306 OLED screen, and utilizes a state-machine with thermal hysteresis to route critical alerts to a Telegram channel.

## 🔑 Key Technical Features
* **Asynchronous Timing Architecture:** Built entirely with non-blocking `millis()` timing loops instead of hard-coded `delay()` functions. This allows the microcontroller to simultaneously handle sensor polling, display rendering, and network requests without freezing the processor.
* **Thermal Hysteresis Logic:** Implements a state buffer zone (1°C threshold variation) to eliminate fluctuating sensor noise, preventing the system from spamming the Telegram API with repetitive notifications when the room temperature hovers on a boundary line.
* **Secure Web Client Integration:** Utilizes `WiFiClientSecure` to interface safely with the HTTPS Telegram Bot API endpoint, allowing the system to process incoming remote commands like `/status` on demand.

---

## 🔌 Hardware Wiring Guide
For a successful setup, connect your components to the ESP32 development board according to the pinouts below.

### 1. Sensors & Displays

| Component | Component Pin | ESP32 GPIO | Description |
| :--- | :--- | :--- | :--- |
| **DHT11 Sensor** | VCC | 3V3 | 3.3V Power Supply |
| | GND | GND | Ground |
| | DATA / OUT | GPIO 4 | Climate Data Input |
| **SSD1306 OLED** | VCC | 3V3 | 3.3V Power Supply |
| | GND | GND | Ground |
| | SDA | GPIO 21 | I2C Data Line |
| | SCL | GPIO 22 | I2C Clock Line |

### 2. Indicators & Alerts

| Component | Component Pin | ESP32 GPIO | Connection Requirement |
| :--- | :--- | :--- | :--- |
| **White LED** | Anode (+) | GPIO 17 | Inline with 220Ω Resistor (Stable Status) |
| **Blue LED** | Anode (+) | GPIO 16 | Inline with 220Ω Resistor (Too Cold Status) |
| **Red LED** | Anode (+) | GPIO 27 | Inline with 220Ω Resistor (Too Hot Status) |
| **Active Buzzer** | Positive (+) | GPIO 26 | Direct connection (Audible Alert) |
| **Common Ground**| Cathode (-) / GND | GND | Tie all LED/Buzzer ground legs together |

### 🔍 Crucial Hardware Notes
* **DHT11 / DHT22 (4-Pin Sensor Chip):** If you are using the standalone 4-pin sensor chip, you must place a 10 kΩ pull-up resistor between the VCC and DATA lines. This pulls the data line high for stable digital communication.
* **DHT11 / DHT22 (3-Pin PCB Module):** If your sensor is already pre-mounted on a small breakout board, you can skip the resistor—the board has one built-in.

---

## 📚 Required Libraries
To compile this project, make sure you have the following libraries installed in your Arduino IDE:
* `DHT sensor library` by Adafruit
* `Adafruit SSD1306` & `Adafruit GFX Library`
* `UniversalTelegramBot` by Brian Lough

---

## 🗺️ Current Status
* **Phase 1 (Current):** Fully functional alpha prototype deployed on a physical breadboard setup.
* **Phase 2 (Planned):** Migration to a permanent Vero board circuit layout to enhance mechanical stability and physical packaging.



### 🎥 Project Demonstration

*(Click the link below the image to watch the full video with sound)*

👉 [Click here to watch the live video demonstration on LinkedIn!](https://www.linkedin.com/posts/adeleye-dara-235579406_iot-esp32-embeddedsystems-ugcPost-7479179852152217600-mZE8/)



https://github.com/user-attachments/assets/930131ac-e4ec-440a-8b62-85158e4bfbf1



![Physical Hardware Setup](IMG-20260704-WA0048.jpg)

[Click here to watch the full video demonstration with sound on LinkedIn!](https://www.linkedin.com/posts/adeleye-dara-235579406_iot-esp32-embeddedsystems-ugcPost-7479199432232767488-zhvb/?utm_source=share&utm_medium=member_android&rcm=ACoAAGeq0QkBUeIoH3QZdjBUO55cZS6MKxYMXI8)
