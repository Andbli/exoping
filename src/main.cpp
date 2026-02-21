
//Libraries
#include <Arduino.h> // Core Arduino library

#define LED_PIN 5
#define NUM_LEDS 16

#include <FastLED.h> // For LED control
#include "lighthandler.h" // Custom LED animation functions



void setup() {
  ringsetup();
}

void loop() {
  ringSpinner(CRGB::Green, 4, 1);
 
  FastLED.show(); 
}