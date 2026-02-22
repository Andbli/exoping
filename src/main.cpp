
// wm.resetSettings();



//Libraries
#include <Arduino.h> // Core Arduino library
#include <WiFi.h>
#include <WiFiManager.h>

#define LED_PIN 13
#define NUM_LEDS 16

#include <FastLED.h> // For LED control
#include "lighthandler.h" // Custom LED animation functions

// Forward declarations
void WM_setup();

void setup() {

  Serial.begin(115200);
  WM_setup();
  ringsetup();

}

void loop() {
  ringSpinner(CRGB::Green, 4, 1);
 
  FastLED.show(); 
}



void WM_setup() {
  
  WiFiManager wm;
  // wm.resetSettings();

  bool connected = wm.autoConnect("ExoPingAP", "password123"); //Future refactor: Make AP name based on chip ID.

  if (!connected) {
    Serial.println("Failed to connect to WiFi. Restarting...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Connected to WiFi! IP Address: " + WiFi.localIP().toString());
}