#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>

// ===================== WiFi Setup =====================
const char* ssid     = "Cgudilla 2.4G@unifi";
const char* password = "fadhilah79";

// ===================== Telegram Bot Setup =====================
const char* botToken = "8366873696:AAGfN5gOyNYP0alZLtl9avIQF3qodUKWxnw";
const char* chatID   = "314423431";

// ===================== Pin LoRa (untuk T-Beam) =====================
#define LORA_SS   18
#define LORA_RST  23
#define LORA_DIO0 26

// ===================== Function Prototypes =====================
void connectInternet();
void sendTelegramMessage(String message);

// ===================== Setup =====================
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n=== LoRa Receiver + Telegram Bot ===");

  connectInternet();

  // LoRa setup
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(923E6)) {
    Serial.println("❌ Starting LoRa failed!");
    while (1);
  }

  Serial.println("✅ LoRa init OK!");
  Serial.println("Menunggu mesej daripada transmitter...\n");
}

// ===================== Loop =====================
void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.println("=== DATA DITERIMA ===");
    Serial.println(incoming);
    Serial.println("======================");

    sendTelegramMessage(incoming);
  }
}

// ===================== WiFi Connect =====================
void connectInternet() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  unsigned long timeout = millis() + 15000;
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi connection failed.");
  }
}

// ===================== Telegram Send =====================
void sendTelegramMessage(String message) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi disconnected, reconnecting...");
    connectInternet();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("❌ Gagal reconnect WiFi.");
      return;
    }
  }

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage";
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"chat_id\":\"" + String(chatID) + "\",\"text\":\"" + message + "\"}";
  int httpCode = http.POST(payload);

  if (httpCode > 0) {
    Serial.printf("✅ Telegram sent (%d): %s\n", httpCode, message.c_str());
  } else {
    Serial.printf("❌ Telegram send error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
