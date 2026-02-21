
// ============================================================
// ringClear(); //Call this to clear the ring immediately when switching animations. Otherwise, the previous animation will fade out on its own.
// ringSpinner(CRGB::Green, 4, 1); //Color, arc width, rotations per minute
// ringPulse(CRGB::DeepPink, 30, 10, 220); //Color, pulses per minute, minimum brightness, maximum brightness (Ta vare på! Fade)
// ringClockFill(CRGB::Orange, 25); //Color, interval in milliseconds per LED step. (Ta vare på! Veldig kul, og ser ut som en loading circle. Passer godt til å vise at noe skjer, og er ikke for lik ringSpinner)
// ringOppositeDots(CRGB::Red, CRGB::Blue, 10, 2); //Color A, Color B, rotations per minute, dot width in LEDs (Ta vare på! Veldig kul!)  // ringCometChase(3, CRGB::Purple, 2, 20); //Number of comets, comet color, tail length in LEDs, chase speed in RPM (Ta vare på! Kanskje kul som custom loading?)
// FastLED.show(); //Must be called to update the LEDs with the animation changes. Call this once per loop() after calling any of theabove animation functions.
// ============================================================
//  lighthandler.h  —  Non-blocking FastLED Ring Animations
//  Optimized for circular LED rings (WS2812B, SK6812, etc.)
//
//  USAGE:
//    1. Define LED_PIN and NUM_LEDS in main.cpp
//    2. #include "lighthandler.h"  (after #include <FastLED.h>)
//    3. Call ringsetup() in setup()
//    4. Call any animation in loop(), then FastLED.show()
//
//  EXAMPLE:
//    void loop() {
//      ringSpinner(CRGB::Cyan, 6, 30);
//      FastLED.show();
//    }
//
//  ALL ANGLES are in degrees (0–359) where 0 = LED #0 (12 o'clock)
//  ALL SPEEDS  are in BPM  (beats per minute)  unless noted
//  ALL WIDTHS  are in number of LEDs
// ============================================================

#pragma once
#include <FastLED.h>

CRGB leds[NUM_LEDS];

static CRGB*    _ring    = nullptr;
static uint16_t _ringLen = 0;

// Call once in setup() before using any ring animations
inline void setupRing(CRGB* ledArray, uint16_t count) {
  _ring    = ledArray;
  _ringLen = count;
}

// Internal: wrap any LED index around the ring
inline uint16_t _wrap(int16_t i) {
  return ((i % (int16_t)_ringLen) + _ringLen) % _ringLen;
}

// Internal: map a 0–359 degree angle to a LED index
inline uint16_t _degToLed(float deg) {
  return _wrap((int16_t)((deg / 360.0f) * _ringLen));
}

// Clear the ring immediately (call when switching animations)
inline void ringClear() {
  fill_solid(_ring, _ringLen, CRGB::Black);
}

// Setup the ring LEDs and FastLED
inline void ringsetup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(180);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  setupRing(leds, NUM_LEDS);
}


// ============================================================
//  1. SPINNER
//  A bright arc of LEDs rotates continuously around the ring,
//  leaving a fading trail behind it — like a loading spinner.
//
//  color     : spinner color
//  arcWidth  : how many LEDs are in the bright arc (2–10)
//  rpm       : rotations per minute (5–60)
//
//  Call: ringSpinner(CRGB::Cyan, 6, 20);
// ============================================================
void ringSpinner(CRGB color = CRGB::Cyan, uint8_t arcWidth = 5, uint8_t rpm = 20) {
  EVERY_N_MILLISECONDS(16) {
    float bpm          = rpm * 360.0f / 60.0f;   // degrees per second
    uint16_t headPos   = (uint32_t)(millis() * bpm / 1000.0f) % _ringLen;

    fadeToBlackBy(_ring, _ringLen, 40);

    for (uint8_t i = 0; i < arcWidth; i++) {
      uint8_t brightness = 255 - (i * (255 / arcWidth));
      _ring[_wrap(headPos - i)] += CRGB(color).nscale8(brightness);
    }
  }
}



// ============================================================
//  3. PULSE RING
//  The entire ring breathes in and out with one color,
//  like a heartbeat or ambient glow.
//
//  color       : pulse color
//  bpm         : pulses per minute (10–80)
//  minBright   : dimmest point (0–100)
//  maxBright   : brightest point (150–255)
//
//  Call: ringPulse(CRGB::DeepPink, 30, 10, 220);
// ============================================================
void ringPulse(CRGB color = CRGB::DeepPink, uint8_t bpm = 30,
               uint8_t minBright = 10, uint8_t maxBright = 220) {
  EVERY_N_MILLISECONDS(20) {
    uint8_t bright = beatsin8(bpm, minBright, maxBright);
    CRGB c         = color;
    c.nscale8(bright);
    fill_solid(_ring, _ringLen, c);
  }
}



// ============================================================
//  5. CLOCK FILL
//  LEDs light up clockwise one by one until the ring is full,
//  then erase counter-clockwise. Like a loading circle.
//
//  color    : fill color
//  interval : milliseconds per LED step (10–80)
//
//  Call: ringClockFill(CRGB::Orange, 25);
// ============================================================
void ringClockFill(CRGB color = CRGB::Orange, uint16_t interval = 25) {
  static int16_t pos    = 0;
  static bool    filling = true;
  EVERY_N_MILLISECONDS_I(clockFillTimer, 25) {
    clockFillTimer.setPeriod(interval);
    if (filling) {
      _ring[_wrap(pos)] = color;
      pos++;
      if (pos >= _ringLen) { pos = _ringLen - 1; filling = false; }
    } else {
      _ring[_wrap(pos)] = CRGB::Black;
      pos--;
      if (pos < 0) { pos = 0; filling = true; }
    }
  }
}


// ============================================================
//  9. OPPOSITE DOTS
//  Two dots always sit exactly opposite each other on the ring
//  and orbit together. Looks great on symmetrical rings.
//
//  colorA    : color of first dot
//  colorB    : color of opposite dot (use same for mirror effect)
//  rpm       : orbit speed
//  dotWidth  : how wide each dot glows (1–4)
//
//  Call: ringOppositeDots(CRGB::Red, CRGB::Blue, 10, 2);
// ============================================================
void ringOppositeDots(CRGB colorA = CRGB::Red, CRGB colorB = CRGB::Blue,
                      uint8_t rpm = 10, uint8_t dotWidth = 2) {
  EVERY_N_MILLISECONDS(16) {
    float degsPerMs = (rpm * 360.0f) / 60000.0f;
    uint16_t posA   = (uint16_t)(millis() * degsPerMs / 360.0f * _ringLen) % _ringLen;
    uint16_t posB   = _wrap(posA + _ringLen / 2);   // always opposite

    fadeToBlackBy(_ring, _ringLen, 50);

    for (int8_t i = -dotWidth; i <= dotWidth; i++) {
      uint8_t bright = 255 - (abs(i) * 60);
      _ring[_wrap(posA + i)] += CRGB(colorA).nscale8(bright);
      _ring[_wrap(posB + i)] += CRGB(colorB).nscale8(bright);
    }
  }
}



// ============================================================
//  11. COMET CHASE
//  Multiple evenly-spaced comets chase each other
//  around the ring in formation.
//
//  numComets : how many comets (1–6, try 2 or 3)
//  color     : comet color
//  tailLength: how many LEDs the tail covers (3–12)
//  rpm       : chase speed
//
//  Call: ringCometChase(3, CRGB::White, 8, 20);
// ============================================================
void ringCometChase(uint8_t numComets = 3, CRGB color = CRGB::White,
                    uint8_t tailLength = 8, uint8_t rpm = 20) {
  EVERY_N_MILLISECONDS(16) {
    float degsPerMs  = (rpm * 360.0f) / 60000.0f;
    uint16_t basePos = (uint16_t)(millis() * degsPerMs / 360.0f * _ringLen) % _ringLen;
    uint16_t spacing = _ringLen / numComets;

    fadeToBlackBy(_ring, _ringLen, 40);

    for (uint8_t c = 0; c < numComets; c++) {
      uint16_t head = _wrap(basePos + c * spacing);
      _ring[head]   = color;
      for (uint8_t t = 1; t < tailLength; t++) {
        uint8_t bright = 255 - (t * (255 / tailLength));
        _ring[_wrap(head - t)] += CRGB(color).nscale8(bright);
      }
    }
  }
}
