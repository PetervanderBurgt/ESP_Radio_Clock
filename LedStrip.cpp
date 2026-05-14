#include "LedStrip.h"
#include <FastLED.h>

#define DATA_PIN 4
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];


void LedStrip::setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(32);
}

void LedStrip::write() {
  //   for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
  //     // Turn our current led on to white, then show the leds
  //     leds[whiteLed] = CRGB::White;

  //     // Show the leds (only one of which is set to white, from above)
  //     FastLED.show();
  //   }
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;

  FastLED.show();
}