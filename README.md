# Emergency Button

Project ini adalah tombol darurat berbasis ESP8266 yang akan mengirim notifikasi saat tombol ditekan melalui MQTT dan Telegram.

## Fitur

- Koneksi Wi-Fi otomatis.
- Publish status tombol ke broker MQTT.
- Kirim pesan darurat ke grup Telegram.
- Konfigurasi sensitif disimpan di `src/config.h` dan tidak ikut masuk repo.

## Struktur Project

- `src/main.cpp` - logic utama device.
- `src/config_public.h` - template konfigurasi.
- `src/config.h` - konfigurasi lokal milik masing-masing device.
- `platformio.ini` - konfigurasi PlatformIO.

## Setup

1. Buka project ini di PlatformIO.
2. Salin `src/config_public.h` menjadi `src/config.h`.
3. Isi data berikut di `src/config.h`:
   - Wi-Fi SSID dan password.
   - MQTT server, port, username, dan password jika dipakai.
   - Telegram bot token dan chat ID.
   - Nama device, nama user, dan topik MQTT.
   - Pin tombol darurat jika berbeda dari default.

## Build dan Upload

Gunakan target `esp12e` yang sudah diset di `platformio.ini`.

```bash
pio run
pio run --target upload
pio device monitor
```

## Catatan

- `src/config.h` sengaja di-ignore supaya kredensial tidak ikut ter-push ke repository.
- Kalau mau membagikan contoh konfigurasi, gunakan `src/config_public.h` sebagai acuan.
