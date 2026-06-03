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
  static void onClick();
  static void onDoubleClick();
  static void onLongPressStart();

  void registerEncoderChangedCallback(GlobalStates state, EncoderCallback cb);
  void unregisterEncoderChangedCallback(GlobalStates state);

  void registerSingleClickCallback(GlobalStates state, CallbackFunction cb);
  void unregisterSingleClickCallback(GlobalStates state);

  void registerDoubleClickCallback(GlobalStates state, CallbackFunction cb);
  void unregisterDoubleClickCallback(GlobalStates state);

  void registerLongPressCallback(GlobalStates state, CallbackFunction cb);
  void unregisterLongPressCallback(GlobalStates state);

private:

  AiEsp32RotaryEncoder aiRotaryEncoder;
  OneButton button;
  static RotaryEncoder* instance;

  int lastEncoderValue;

  enum ButtonEvent {
    CLICK,
    DOUBLE_CLICK,
    LONG_PRESS
  };

  static void buttonEvent(ButtonEvent event);

  void handleButtonEvent(ButtonEvent event);

  // state callbacks
  EncoderCallback encoderChangedCallbacks[2];
  CallbackFunction singleClickCallbacks[2];
  CallbackFunction doubleClickCallbacks[2];
  CallbackFunction longPressCallbacks[2];
};

#endif