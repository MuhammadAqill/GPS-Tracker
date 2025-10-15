
// transmeter

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <LoRa.h>
#include <math.h>

// ===================== GPS Setup (T-Beam) =====================
HardwareSerial GPS(1);
TinyGPSPlus gps;

#define R 6371000.0 // jejari bumi dalam meter
#define MOVEMENT_THRESHOLD 10 // meter

#define GPS_RX 34   // RX2 pada T-Beam
#define GPS_TX 12   // TX2 pada T-Beam

// Pin lalai untuk T-Beam (SX1276)
#define LORA_SS   18
#define LORA_RST  23   // ada versi 39, cuba 23 dulu
#define LORA_DIO0 26

#define GPS_BAUD 9600

// ===================== Flags & Data =====================
bool gpsEnabled = false;

String messageLat = "";
String messageLon = "";

float lastLat = 3.456863;
float lastLon = 101.669746;

bool hasLastFix = false;  // supaya tak kira jarak kali pertama

// ===================== Forward Declarations =====================
void initializeGPS();
void gpsTask(void *pvParameters);

// ===================== Setup =====================
void setup() {
  Serial.begin(115200);
  delay(2000);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(923E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("");
  Serial.println("LoRa init OK!");
  Serial.println("LilyGO T-Beam GPS Tracker (Auto Send Outside Zone)");

  initializeGPS();
  

  xTaskCreatePinnedToCore(gpsTask, "gpsTask", 8192, NULL, 1, NULL, 0);
}

void loop() {
  // handled by FreeRTOS tasks
}


// ===================== GPS Init =====================
void initializeGPS() {
  Serial.println("Initializing GPS");
  GPS.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
  delay(2000);
  gpsEnabled = true;
  Serial.println("GPS initialized.");
  Serial.println("");
}

// ===================== Haversine Function =====================
double haversine(double lat1, double lon1, double lat2, double lon2) {
  double dLat = radians(lat2 - lat1);
  double dLon = radians(lon2 - lon1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);

  double a = pow(sin(dLat / 2), 2) +
             cos(lat1) * cos(lat2) * pow(sin(dLon / 2), 2);
  double c = 2 * asin(sqrt(a));
  return R * c; // meter
}


// ===================== GPS Task =====================
void gpsTask(void *pvParameters) {
  while (1) {
    if (gpsEnabled) {
      while (GPS.available() > 0) {
        gps.encode(GPS.read());
      }

      if (gps.location.isValid()) {
        if(hasLastFix) {
          float currentLat = gps.location.lat();
          float currentLon = gps.location.lng();

          double jarak = haversine(lastLat, lastLon, currentLat, currentLon);

          messageLat = "Latitude: " + String(currentLat, 6);
          messageLon = "Longitude: " + String(currentLon, 6);

          Serial.println("GPS DATA");
          Serial.printf("📏 Jarak dari lokasi sebelumnya: %.2f meter\n", jarak);
          Serial.println(messageLat);
          Serial.println(messageLon);
          Serial.printf("Google Maps: https://www.google.com/maps?q=%.6f,%.6f\n", currentLat, currentLon);

          if (jarak >= MOVEMENT_THRESHOLD) {
            Serial.println("GPS DATA");
            Serial.println("🚨 Terdapat pergerakan lebih dari 10 meter!");
            Serial.printf("📏 Jarak dari lokasi sebelumnya: %.2f meter\n", jarak);
            Serial.println(messageLat);
            Serial.println(messageLon);
            Serial.printf("Google Maps: https://www.google.com/maps?q=%.6f,%.6f\n", currentLat, currentLon);

            LoRa.beginPacket();
            LoRa.println("ALERT: Pergerakan dikesan!");
            LoRa.print("📏 Jarak dari lokasi sebelumnya:");
            LoRa.println(jarak);
            LoRa.println(messageLat);
            LoRa.println(messageLon);
            LoRa.printf("Google Maps: https://www.google.com/maps?q=%.6f,%.6f\n", currentLat, currentLon);
            LoRa.endPacket();

            Serial.println("📡 Data dihantar ke receiver bagi pergerakan.\n");

          } else if (jarak < MOVEMENT_THRESHOLD) {
            LoRa.beginPacket();
            LoRa.println("Tiada Pergerakan dikesan!");
            LoRa.print("📏 Jarak dari lokasi sebelumnya:");
            LoRa.println(jarak);
            LoRa.println(messageLat);
            LoRa.println(messageLon);
            LoRa.printf("Google Maps: https://www.google.com/maps?q=%.6f,%.6f\n", currentLat, currentLon);
            LoRa.endPacket();

            Serial.println("📡 Data dihantar ke receiver bagi tiada pergerakan.\n");

          }

        }

        hasLastFix = true;

      } else {
        Serial.println("Waiting for GPS fix...");
      }
    }

    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Update setiap 10s
  }
}