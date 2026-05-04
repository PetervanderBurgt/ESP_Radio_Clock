#include "WifiManagerWrapper.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

void WifiManagerWrapper::setup_wifi() {
    WiFiManager wm;

    // Optional: reset settings (for testing)
    // wm.resetSettings();
    // Auto connect
    if (!wm.autoConnect("ESP_Radio_Clock")) {
        // Failed to connect or timeout reached
        Serial.println("Failed to connect. Restarting...");
        delay(3000);
        ESP.restart();
    }

    // If we get here, we're connected
    Serial.println("Connected to WiFi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}