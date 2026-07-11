#pragma once
#include <Arduino.h>

// --- Kredensial WiFi ---
const char* WIFI_SSID     = "wifi_ssid";
const char* WIFI_PASSWORD = "wifi_password";

// --- Kredensial Telegram ---
const String BOT_TOKEN    = "bot token";
const String CHAT_ID      = " chat id";

// --- Konfigurasi Device Publisher ---
const String DEVICE_ID    = "device:x";
const String USER_NAME    = "user";

// --- Emergency endpoint (server lokal) ---
const char* EMERGENCY_URL = "url:port/emergency";
const char* API_KEY       = "";   // isi kalau server pakai X-API-Key, kosongin kalau tidak

// --- Pinout ESP8266/Wemos ---
const int BUTTON_PIN      = 4; // Pin Emergency Button