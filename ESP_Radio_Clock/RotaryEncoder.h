#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "GlobalVariables.h"
#include "AiEsp32RotaryEncoder.h"
#include <OneButton.h>

class RotaryEncoder {
public:
  RotaryEncoder();

  void setup();
  void rotary_loop();

  static void readEncoderISR();

  // OneButton required wrappers
  static void onEncoderChanged(int value, int delta);
  static void onClick();
  static void onDoubleClick();
  static void onLongPressStart();

private:
    static RotaryEncoder* instance;
    
  AiEsp32RotaryEncoder aiRotaryEncoder;
  OneButton button;

  int lastEncoderValue;

  enum ButtonEvent {
    CLICK,
    DOUBLE_CLICK,
    LONG_PRESS
  };

  static void buttonEvent(ButtonEvent event);

  void handleButtonEvent(ButtonEvent event);
};

#endif