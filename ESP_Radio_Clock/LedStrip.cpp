#include "LedStrip.h"
#include <FastLED.h>
#include "GlobalVariables.h"
#include <BasicTimer.h>

#define DATA_PIN 15
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];

extern GlobalStates global_state;
extern ConfigStates config_state;
extern bool alarm_on[7];

BasicBlinker basicBlinker(500);

void LedStrip::setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(32);
  basicBlinker.reset();
}

void LedStrip::loop() {
  FastLED.clear();
  // Use the basicblinker for blinking status leds
  basicBlinker.run();

  int day_number = int(config_state / 3);
  switch (global_state) {
    case clock_config:
      if (config_state % 3 == 0) {
        leds[day_number] = basicBlinker ? CRGB::Green : CRGB::Black;
      } else if (config_state % 3 == 1) {
        leds[day_number] = basicBlinker ? CRGB::Blue : CRGB::Black;
      } else if (config_state % 3 == 2) {
        leds[day_number] = basicBlinker ? CRGB::Red : CRGB::Black;
      }
      break;
    case clock_on:
    default:
      setAlarm();
      break;
  }
  FastLED.show();
}

void LedStrip::setAlarm() {
  for (int weekday = 0; weekday < 7; weekday++) {
    if (alarm_on[weekday]) leds[weekday] = CRGB::Red;
  }
}