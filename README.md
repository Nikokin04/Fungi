# Fungi
Fungi is a compact and powerful indoor air quality monitoring system developed using an ESP32, originally designed in the context of the COVID-19 pandemic to support occupancy control in enclosed spaces. It integrates multiple sensors and communication protocols to measure air quality and transmit data in real time, helping to assess the risk of airborne transmission and improve ventilation strategies.

⚙️ Features:
🌫 Particulate Matter (PM1.0 / PM2.5 / PM10) monitoring via PMS1003
🌡️ Temperature and Humidity readings via AM2320
🟩 CO₂ concentration monitoring to assess ventilation quality
📍 GPS tracking using u-blox NEO-M8N for location-based logging
🕒 Real-Time Clock via NTPClient and external RTC module
💾 Local data logging to microSD card, accessible via FTP server
🔒 Wi-Fi credentials configuration via ESPAsyncWebServer (captive portal)
🌐 MQTT publishing to a custom Mosquitto server using JSON payloads
⚙️ Task-oriented architecture using Esp_Task_Wdt
💡 EEPROM used for storing persistent configurations

🔧 Hardware:
- Microcontroller: ESP32
- Sensors: PMS1003 (PM), AM2320 (Temp/Humidity), CO₂ sensor
- GPS Module: u-blox NEO-M8N
- Storage: microSD card module + EEPROM
- RTC: External RTC module
- Connectivity: Wi-Fi, MQTT (PubSubClient), FTP server
