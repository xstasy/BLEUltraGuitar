#include "ota.h"

#include <Arduino.h>
#include <WiFiManager.h>   // tzapu/WiFiManager — handles captive portal automatically
#include <ArduinoOTA.h>

void enterOtaMode() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n\n--- OTA MODE ---");

    // WiFiManager tries the last saved network first.
    // If it cannot connect, it raises an AP called "BLEUltraGuitar-OTA"
    // and serves a captive portal so the user can enter new credentials.
    WiFiManager wm;
    //wm.resetSettings();                // reset saved credentials to ensure we always get a captive portal in OTA mode, even if credentials were previously saved
    wm.setConnectTimeout(20);          // 20 s to connect before falling back to portal
    wm.setConfigPortalTimeout(180);    // portal auto-closes after 3 min if unused

    bool connected = wm.autoConnect("BLEUltraGuitar-OTA");
    if (!connected) {
        // Portal timed out without receiving credentials — restart and try again.
        Serial.println("WiFi config portal timed out. Rebooting...");
        delay(1000);
        ESP.restart();
    }

    Serial.print("WiFi connected. IP: ");
    Serial.println(WiFi.localIP());

    // ArduinoOTA — PlatformIO will discover the device via mDNS as "BLEUltraGuitar.local"
    ArduinoOTA.setHostname("BLEUltraGuitar");

    ArduinoOTA.onStart([]() {
        Serial.println("OTA upload starting...");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA upload complete. Rebooting...");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress * 100) / total);
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA error [%u]\n", error);
    });

    ArduinoOTA.begin();
    Serial.println("OTA ready — waiting for upload from PlatformIO...");

    // Spin forever; normal guitar code is never reached.
    while (true) {
        ArduinoOTA.handle();
        delay(10);
    }
}
