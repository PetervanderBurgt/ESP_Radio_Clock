#include "RotaryEncoder.h"
#include "AiEsp32RotaryEncoder.h"
#include <OneButton.h>

#define ROTARY_ENCODER_A_PIN D6
#define ROTARY_ENCODER_B_PIN D5
#define ROTARY_ENCODER_BUTTON_PIN D7
#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */


//depending on your encoder - try 1,2 or 4 to get expected behaviour
// #define ROTARY_ENCODER_STEPS 1
// #define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4

//set boundaries and if values should cycle or not
#define MIN_ENCODER_VALUE 0
#define MAX_ENCODER_VALUE 60
#define LOOP_BACK_VALUE true

/*Rotary acceleration introduced 25.2.2021.
   * in case range to select is huge, for example - select a value between 0 and 1000 and we want 785
   * without accelerateion you need long time to get to that number
   * Using acceleration, faster you turn, faster will the value raise.
   * For fine tuning slow down.
   */
//aiRotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
//or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
#define ROTARY_ENCODER_ACCELERATION 0


#define SINGLE_BUTTON_CLICK_MS 250
// Quite long to ensure not accidentally accessing Config
#define LONG_BUTTON_CLICK_MS 2000

extern GlobalStates global_state;

//instead of changing here, rather change numbers above
// Set Button to -1 as this will be handled by OneButton


RotaryEncoder* RotaryEncoder::instance = nullptr;



RotaryEncoder::RotaryEncoder()
  : aiRotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, -1, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS),
    button(ROTARY_ENCODER_BUTTON_PIN, true) {
  instance = this;
  memset(singleClickCallbacks, 0, sizeof(singleClickCallbacks));
  memset(doubleClickCallbacks, 0, sizeof(doubleClickCallbacks));
  memset(longPressCallbacks, 0, sizeof(longPressCallbacks));
  lastEncoderValue = 0;
}

void RotaryEncoder::setup() {
  //we must initialize rotary encoder
  aiRotaryEncoder.begin();
  aiRotaryEncoder.setup(readEncoderISR);
  aiRotaryEncoder.setBoundaries(MIN_ENCODER_VALUE, MAX_ENCODER_VALUE, LOOP_BACK_VALUE);  //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

  aiRotaryEncoder.setAcceleration(ROTARY_ENCODER_ACCELERATION);

  button.setClickMs(SINGLE_BUTTON_CLICK_MS);
  button.setPressMs(LONG_BUTTON_CLICK_MS);

  button.attachClick(onClick);
  button.attachDoubleClick(onDoubleClick);
  button.attachLongPressStart(onLongPressStart);
}

void RotaryEncoder::rotary_loop() {
  button.tick();

  if (aiRotaryEncoder.encoderChanged()) {

    int current = aiRotaryEncoder.readEncoder();
    int delta = current - lastEncoderValue;

    lastEncoderValue = current;

    Serial.print("Value: ");
    Serial.print(current);

    if (delta > 0) {
      Serial.println(" (UP)");
    } else if (delta < 0) {
      Serial.println(" (DOWN)");
    }
    if (encoderChangedCallbacks[global_state]) {
      encoderChangedCallbacks[global_state](delta);
    }
  }
}

void IRAM_ATTR RotaryEncoder::readEncoderISR() {
  if (instance) {
    instance->aiRotaryEncoder.readEncoder_ISR();
  }
}

void RotaryEncoder::onClick() {
  buttonEvent(CLICK);
}

void RotaryEncoder::onDoubleClick() {
  buttonEvent(DOUBLE_CLICK);
}

void RotaryEncoder::onLongPressStart() {
  buttonEvent(LONG_PRESS);
}

void RotaryEncoder::buttonEvent(ButtonEvent event) {
  if (instance) {
    instance->handleButtonEvent(event);
  }
}

void RotaryEncoder::handleButtonEvent(ButtonEvent event) {

  Serial.print("Button event: ");
  Serial.println(event);

  switch (event) {

    case CLICK:
      if (singleClickCallbacks[global_state]) {
        singleClickCallbacks[global_state]();
      }
      break;

    case DOUBLE_CLICK:
      if (doubleClickCallbacks[global_state]) {
        doubleClickCallbacks[global_state]();
      }
      break;

    case LONG_PRESS:
      if (longPressCallbacks[global_state]) {
        longPressCallbacks[global_state]();
      }
      break;
  }
}

void RotaryEncoder::registerEncoderChangedCallback(GlobalStates state, EncoderCallback cb) {
  encoderChangedCallbacks[state] = cb;
}
void RotaryEncoder::unregisterEncoderChangedCallback(GlobalStates state) {
  encoderChangedCallbacks[state] = nullptr;
}
void RotaryEncoder::registerSingleClickCallback(GlobalStates state, CallbackFunction cb) {
  singleClickCallbacks[state] = cb;
}
void RotaryEncoder::unregisterSingleClickCallback(GlobalStates state) {
  singleClickCallbacks[state] = nullptr;
}
void RotaryEncoder::registerDoubleClickCallback(GlobalStates state, CallbackFunction cb) {
  doubleClickCallbacks[state] = cb;
}
void RotaryEncoder::unregisterDoubleClickCallback(GlobalStates state) {
  doubleClickCallbacks[state] = nullptr;
}
void RotaryEncoder::registerLongPressCallback(GlobalStates state, CallbackFunction cb) {
  longPressCallbacks[state] = cb;
}
void RotaryEncoder::unregisterLongPressCallback(GlobalStates state) {
  longPressCallbacks[state] = nullptr;
}