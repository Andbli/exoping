
// wm.resetSettings();



//Libraries
#include <Arduino.h> // Core Arduino library
#include <WiFi.h>
#include <WiFiManager.h>
#include "Button2.h"

#define LED_PIN 13
#define NUM_LEDS 16
#define TPSENSOR1_PIN 14

String chipID;
String deviceName;

Button2 tpsensor1;

#include <FastLED.h> // For LED control
#include "lighthandler.h" // Custom LED animation functions

// Forward declarations
void WM_setup();
void tpSensor1();




String getWMChipID() {
    String mac = WiFi.macAddress();   // "AA:BB:CC:DD:EE:FF"

    mac.replace(":", "");             // "AABBCCDDEEFF"
    mac.toLowerCase();                // match WM format

    return mac.substring(4);          // last 8 hex chars
}

WiFiManager wm;   // declare GLOBALLY for on-demand use


void setup() {

  Serial.begin(115200);
  chipID = getWMChipID();
  deviceName = "ExoPing_" + chipID;

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
  wm.setEnableConfigPortal(false);
  bool connected = wm.autoConnect("MyDevice");
  if (!connected) Serial.println("No saved credentials — press button to configure");

}




void tpSensor1() {
  //Handles tpsensor1!

  //Handle button click event for tpsensor1
  tpsensor1.setClickHandler([](Button2& b) {
    
    Serial.println("TPSensor1 clicked");  
  });

  //Handle button long click event for tpsensor1
  tpsensor1.setLongClickHandler([](Button2& b) {
    
    Serial.println("Opening WiFi config portal...");
    wm.setClass("invert");
    wm.setConfigPortalTimeout(120);
 
    wm.startConfigPortal(deviceName.c_str(), "password123");
    Serial.println("WiFi Portal has been closed!");
    
  });

  //Handle button double click event for tpsensor1
  tpsensor1.setDoubleClickHandler([](Button2& b) {
    
    
    Serial.println("TPSensor1 double clicked"); 
  });

}