#include "secrets.h"

// Constants
#define LED_PIN 13
#define NUM_LEDS 16
const int TPSENSOR1_PIN = 14;

const float fwversion = 0.1; //Firmware version device.
#define devicefnName "ExoPing" //Friendly name. //Todo: Allow user to set this in config portal and save in SPIFFS or EEPROM.
int devicecurrentmode = 0; //Current mode of the device, used for MQTT state reporting. Todo: Implement this to accualy change in code.
#define charging true; //Charging state of the device, used for MQTT state reporting. Todo: Implement this to accualy change in code.
int battery = 100; //Battery level of the device, used for MQTT state reporting. Todo: Implement this to accualy change in code.

// Libraries
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <FastLED.h>
#include "Button2.h"
#include "lighthandler.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"

// Global variables
String chipID;
String deviceName;
Button2 tpsensor1;
WiFiManager wm;

// Function to get chip ID from MAC address
String getWMChipID() {
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    mac.toLowerCase();
    return mac.substring(4);
}

// Setup WiFi Manager
void WM_setup() {
    wm.setEnableConfigPortal(false);
    bool connected = wm.autoConnect(deviceName.c_str());
    if (!connected) {
        Serial.println("No saved credentials — press button to configure");
    }
}

// Setup touch sensor handlers
void tpSensor1() {
    tpsensor1.setClickHandler([](Button2& b) {
        Serial.println("TPSensor1 clicked");
    });

    tpsensor1.setLongClickHandler([](Button2& b) {
        Serial.println("Opening WiFi config portal...");
        wm.setClass("invert");
        wm.setConfigPortalTimeout(120);
        wm.startConfigPortal(deviceName.c_str(), "password123");
        Serial.println("WiFi Portal has been closed!");
    });

    tpsensor1.setDoubleClickHandler([](Button2& b) {
        Serial.println("TPSensor1 double clicked");
    });
}

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