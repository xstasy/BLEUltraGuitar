#pragma once

/**
 * enterOtaMode()
 *
 * Called at boot when SELECT + START are held simultaneously.
 * Connects to WiFi (captive portal on first run or unknown network),
 * starts ArduinoOTA and loops forever — normal guitar code never runs.
 *
 * Flash via PlatformIO:  pio run -e BLEUltraGuitar-OTA -t upload
 */
void enterOtaMode();
