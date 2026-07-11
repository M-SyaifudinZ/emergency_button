#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "config.h"

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 500;

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected! IP: ");
  Serial.println(WiFi.localIP());
}

void sendTelegramGroup(String message) {
  std::unique_ptr<BearSSL::WiFiClientSecure> teleClient(new BearSSL::WiFiClientSecure());
  teleClient->setInsecure();
  teleClient->setBufferSizes(1024, 512);

  Serial.println("Mengirim ke Telegram (POST JSON)...");
  if (teleClient->connect("api.telegram.org", 443)) {

    String payload = "{\"chat_id\":\"" + CHAT_ID + "\",\"text\":\"" + message + "\"}";
    String url = "/bot" + BOT_TOKEN + "/sendMessage";

    teleClient->print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: api.telegram.org\r\n" +
                       "Content-Type: application/json\r\n" +
                       "Content-Length: " + String(payload.length()) + "\r\n" +
                       "Connection: close\r\n\r\n" +
                       payload);

    unsigned long timeout = millis();
    while (teleClient->available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> [ERROR] Timeout!");
        teleClient->stop();
        return;
      }
    }

    Serial.println(">>> Balasan Detail dari Telegram:");
    while (teleClient->available()) {
      String line = teleClient->readStringUntil('\n');
      Serial.println(line);
    }
    Serial.println("---------------------------------");

    teleClient->stop();
  } else {
    Serial.println("Gagal konek ke API Telegram!");
  }
}

// --- Broadcast emergency ke server lokal (plain HTTP, no TLS) ---
void postEmergency() {
  WiFiClient client;
  HTTPClient http;

  Serial.println("Mengirim emergency ke server lokal...");
  if (http.begin(client, EMERGENCY_URL)) {
    http.addHeader("Content-Type", "application/json");
    if (strlen(API_KEY) > 0) {
      http.addHeader("X-API-Key", API_KEY);
    }
    http.setTimeout(3000);

    String payload = "{\"device\":\"" + DEVICE_ID + "\"}";
    int code = http.POST(payload);

    if (code > 0) {
      Serial.printf("[EMG] POST response: %d\n", code);
    } else {
      Serial.printf("[EMG] POST gagal: %s\n", http.errorToString(code).c_str());
    }
    http.end();
  } else {
    Serial.println("[EMG] Gagal konek ke server emergency!");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Sistem Publisher Mulai ---");

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  setup_wifi();
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      Serial.println("\n[!] TOMBOL DITEKAN [!]");

      String teleMsg = "🚨 DARURAT! Tolong bantuan di rumah " + USER_NAME + " (" + DEVICE_ID + ")";
      sendTelegramGroup(teleMsg);
      postEmergency();

      lastDebounceTime = millis();
    }
  }
}