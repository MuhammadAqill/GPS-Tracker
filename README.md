---

# 🚀 GPS & LoRa Movement Detection System

This project implements a **movement detection system using GPS and LoRa**, built with **LilyGO T-Beam** modules.
The **transmitter** tracks GPS coordinates and transmits data via **LoRa** whenever it detects movement exceeding **4 meters**.
The **receiver** then sends a **notification to Telegram** through **WiFi** using the **Telegram Bot API**.

---

## 🛰️ Features

* Real-time GPS tracking using **TinyGPSPlus**
* Long-range wireless communication via **LoRa (SX1276 @ 923 MHz)**
* Automatic movement detection when distance > 4 meters
* **Telegram notifications** with location and Google Maps link
* **WiFi auto-reconnect** for reliable message delivery
* Compatible with **LilyGO T-Beam ESP32**

---

## ⚙️ Hardware Requirements

| Component           | Quantity | Description                                     |
| ------------------- | -------- | ----------------------------------------------- |
| LilyGO T-Beam ESP32 | 2        | One as transmitter, one as receiver             |
| LoRa Antenna        | 2        | For long-range communication                    |
| GPS Antenna         | 1        | For accurate location detection                 |
| WiFi Network        | 1        | Receiver uses WiFi to send messages to Telegram |

---

## 🧩 Libraries Used

* [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)
* [LoRa](https://github.com/sandeepmistry/arduino-LoRa)
* [WiFi](https://www.arduino.cc/en/Reference/WiFi)
* [HTTPClient](https://www.arduino.cc/en/Reference/HTTPClient)

---

## 🔧 Setup Instructions

### 1️⃣ **Transmitter (T-Beam)**

* Reads GPS coordinates.
* Calculates movement distance using the **Haversine formula**.
* Sends data via LoRa if the distance moved > 4 meters.

### 2️⃣ **Receiver (T-Beam)**

* Listens for LoRa packets from the transmitter.
* Parses incoming GPS coordinates.
* Connects to WiFi and sends location + alert message to **Telegram**.

---

## 📱 Telegram Setup

1. Open Telegram and search for **BotFather**.
2. Create a new bot using `/newbot`.
3. Copy the **Bot Token**.
4. Get your **Chat ID** using [@userinfobot](https://t.me/userinfobot).
5. Add your **Bot Token** and **Chat ID** to the receiver code:

   ```cpp
   const char* botToken = "YOUR_BOT_TOKEN";
   const char* chatID   = "YOUR_CHAT_ID";
   ```

---

## 📡 LoRa Configuration

* Frequency: **923 MHz** (for Malaysia/Southeast Asia)
* Pins (for LilyGO T-Beam):

  ```cpp
  #define LORA_SS   18
  #define LORA_RST  23
  #define LORA_DIO0 26
  ```

---

## 🧠 How It Works

1. **Transmitter** continuously reads GPS data.
2. Calculates distance between **last position** and **current position**.
3. If movement exceeds **4 meters**, transmitter sends new coordinates via LoRa.
4. **Receiver** receives the data and sends:

   * Latitude
   * Longitude
   * Google Maps link
   * Optional alert message ("Movement Detected!")
     to **Telegram**.

---

## 🗺️ Example Telegram Output

```
📍 Latitude: 2.904685
📍 Longitude: 101.864059
🌐 Google Maps:
https://maps.google.com/?q=2.904685,101.864059
⚠️ Movement Detected! Device moved more than 4 meters.
```

---

## 🧮 Distance Formula (Haversine)

The distance between two GPS points is calculated as:
[
d = 2R \times \arcsin\left(\sqrt{\sin^2\left(\frac{Δφ}{2}\right) + \cos(φ1) \cdot \cos(φ2) \cdot \sin^2\left(\frac{Δλ}{2}\right)}\right)
]
Where:

* ( R = 6371000 , m ) (Earth’s radius)
* ( φ1, φ2 ) = latitude in radians
* ( λ1, λ2 ) = longitude in radians

---

## 🧾 License

This project is open-source under the **MIT License**.
Feel free to use, modify, and share with proper attribution.

---

## ✉️ Author

**Muhammad Aqil**
📬 Contact: [Telegram](https://t.me/yourusername) *(optional)*
💡 Project Goal: To create a compact, low-power, and reliable GPS-based movement detection system using LoRa and Telegram integration.

---
