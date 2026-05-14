#include "TM1637Wrapper.h"
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 0
#define DIO 2

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

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