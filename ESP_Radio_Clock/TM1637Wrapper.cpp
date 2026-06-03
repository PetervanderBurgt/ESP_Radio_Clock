#include "TM1637Wrapper.h"
#include <TM1637Display.h>
#include "GlobalVariables.h"

// Module connection pins (Digital Pins)
#define CLK 0
#define DIO 2

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

extern GlobalStates global_state;
extern ConfigStates config_state;

const uint8_t SEG_ON[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_E | SEG_C | SEG_G,                          // n
  0x00,
  0x00
};

const uint8_t SEG_OFF[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_A | SEG_F | SEG_G | SEG_E,                  // F
  SEG_A | SEG_F | SEG_G | SEG_E,                  // F
  0x00
};

void TM1637Wrapper::setup() {
  display.setBrightness(0x0a);  //set the diplay to maximum brightness
}

void TM1637Wrapper::set_time(uint8_t hour, uint8_t minutes, uint8_t seconds) {
  uint16_t timeValue = hour * 100 + minutes;
  if (seconds % 2 == 0) {
    display.showNumberDecEx(timeValue, 0b00000000, true);
  } else {
    display.showNumberDecEx(timeValue, 0b01000000, true);
  }
}

void TM1637Wrapper::loop(tm current_time, tm config_time) {
  switch (global_state) {
    case clock_config:
      if (config_state % 3 == 2) {
        if (config_time.tm_sec == 0) {
          // show off
          display.setSegments(SEG_OFF);  // show "OFF"
        } else {
          //show on
          display.setSegments(SEG_ON);  // show "ON"
        }

      } else {
        set_time(config_time.tm_hour, config_time.tm_min, 1);
      }
      break;
    case clock_on:
    default:
      set_time(current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
      break;
  }
}
