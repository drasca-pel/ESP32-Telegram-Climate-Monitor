# ESP32 Telegram Climate Monitor

A non-blocking, asynchronous environmental monitoring system built on the standard ESP32 platform. The device tracks ambient temperature and humidity in real-time using a DHT11 sensor, displays live metrics locally on an SSD1306 OLED screen, and utilizes a state-machine with thermal hysteresis to route critical alerts to a Telegram channel.

## Hardware Components
* ESP32 Development Board (Standard Module)
* DHT11 Temperature & Humidity Sensor
* 128x64 I2C OLED Display (SSD1306)
* Hardware Warning Buzzer & Status LEDs (Red, Blue, White)

## Key Technical Features
* **Asynchronous Timing Architecture:** Built entirely with non-blocking `millis()` timing loops instead of hard-coded `delay()` functions. This allows the microcontroller to simultaneously handle sensor polling, display rendering, and network requests without freezing the processor.
* **Thermal Hysteresis Logic:** Implements a state buffer buffer zone (1°C threshold variation) to eliminate fluctuating sensor noise, preventing the system from spamming the Telegram API with repetitive notifications when the room temperature hovers on a boundary line.
* **Secure Web Client Integration:** Utilizes `WiFiClientSecure` to interface safely with the HTTPS Telegram Bot API endpoint, allowing the system to process incoming remote commands like `/status` on demand.

## Current Status
* **Phase 1 (Current):** Fully functional alpha prototype deployed on a physical breadboard setup.
* **Phase 2 (Planned):** Migration to a permanent Vero board circuit layout to enhance mechanical stability and physical packaging.
* ### Project Demonstration Video
![Watch the demo](VID-20260629-WA0009(1).mp4)
