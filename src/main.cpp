#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "config.h"


WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

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

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    bool connected = false;
    
    if (String(MQTT_USER).length() > 0) {
      connected = mqttClient.connect(DEVICE_ID.c_str(), MQTT_USER, MQTT_PASS);
    } else {
      connected = mqttClient.connect(DEVICE_ID.c_str());
    }

    if (connected) {
      Serial.println(" MQTT Connected!");
    } else {
      Serial.print(" Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" - trying again in 5 seconds");
      delay(5000);
    }
  }
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
                       
    // --- BACA FULL RESPONSE DARI TELEGRAM ---
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
      Serial.println(line); // Tampilkan semua baris
    }
    Serial.println("---------------------------------");
    
    teleClient->stop();
  } else {
    Serial.println("Gagal konek ke API Telegram!");
  }
}
void setup() {
  Serial.begin(115200); // Mulai komunikasi Serial
  delay(1000);
  Serial.println("\n--- Sistem Publisher Mulai ---");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  setup_wifi();
  espClient.setInsecure();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  // Deteksi Button
  if (digitalRead(BUTTON_PIN) == LOW) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      Serial.println("\n[!] TOMBOL DITEKAN [!]");
      
      // Publish ke HiveMQ
      Serial.print("Publishing MQTT ke topik: ");
      Serial.println(MQTT_TOPIC);
      mqttClient.publish(MQTT_TOPIC, DEVICE_ID.c_str());
      
      // Kirim ke Telegram
      String teleMsg = "🚨 DARURAT! Tolong bantuan di rumah " + USER_NAME + " (" + DEVICE_ID + ")";
      sendTelegramGroup(teleMsg);
      
      lastDebounceTime = millis();
    }
  }
}