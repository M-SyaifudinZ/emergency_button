#pragma once
#include <Arduino.h>

// --- Kredensial WiFi ---
const char* WIFI_SSID     = "wifi_ssid";
const char* WIFI_PASSWORD = "wifi_password";

// --- Kredensial HiveMQ ---
// Jika pakai public broker HiveMQ: "broker.hivemq.com" (Port 1883)
const char* MQTT_SERVER   = "mqtt_server_address"; 
const int   MQTT_PORT     = 8883;
// Kosongkan user & pass jika pakai public broker
const char* MQTT_USER     = "username"; 
const char* MQTT_PASS     = "password"; 

// --- Kredensial Telegram ---
const String BOT_TOKEN    = "bot token";
const String CHAT_ID      = " chat id";

// --- Konfigurasi Device Publisher ---
const String DEVICE_ID    = "device:x";
const String USER_NAME    = "user";
const char*  MQTT_TOPIC   = "topic_name";

// --- Pinout ESP8266/Wemos ---
const int BUTTON_PIN      = 4; // Pin Emergency Button