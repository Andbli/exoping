
// wm.resetSettings();



//Libraries
#include <Arduino.h> // Core Arduino library
#include <WiFi.h>
#include <WiFiManager.h>
#include "Button2.h"

#define LED_PIN 13
#define NUM_LEDS 16
#define TPSENSOR1_PIN 14

Button2 tpsensor1;

#include <FastLED.h> // For LED control
#include "lighthandler.h" // Custom LED animation functions

// Forward declarations
void WM_setup();
void tpSensor1();

void setup() {

  Serial.begin(115200);

  tpsensor1.begin(TPSENSOR1_PIN, INPUT_PULLDOWN, false);
  tpSensor1();

  WM_setup();
  ringsetup();

}

void loop() {
  tpsensor1.loop();
  ringSpinner(CRGB::Green, 4, 1);
 
  FastLED.show(); 
}



void WM_setup() {
  
  WiFiManager wm;
  wm.setClass("invert");
  // wm.resetSettings();

  bool connected = wm.autoConnect("ExoPingAP", "password123"); //Future refactor: Make AP name based on chip ID.

  if (!connected) {
    Serial.println("Failed to connect to WiFi. Restarting...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Connected to WiFi! IP Address: " + WiFi.localIP().toString());
}




void tpSensor1() {
  //Handles tpsensor1!

  //Handle button click event for tpsensor1
  tpsensor1.setClickHandler([](Button2& b) {
    
    Serial.println("TPSensor1 clicked");  
  });

  //Handle button long click event for tpsensor1
  tpsensor1.setLongClickHandler([](Button2& b) {
    
    Serial.println("Resetting WiFi settings...");
    WiFiManager wm;
    wm.resetSettings();
    ESP.restart();
  });

  //Handle button double click event for tpsensor1
  tpsensor1.setDoubleClickHandler([](Button2& b) {
    
    
    Serial.println("TPSensor1 double clicked"); 
  });

}