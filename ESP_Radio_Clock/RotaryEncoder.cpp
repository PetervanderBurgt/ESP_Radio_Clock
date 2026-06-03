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

GlobalStates global_state = clock_on;


RotaryEncoder* RotaryEncoder::instance = nullptr;

RotaryEncoder::RotaryEncoder()
  : aiRotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, -1, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS),
    button(ROTARY_ENCODER_BUTTON_PIN, true) {
  instance = this;
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

    onEncoderChanged(current, delta);
  }
}

void IRAM_ATTR RotaryEncoder::readEncoderISR() {
  if (instance) {
    instance->aiRotaryEncoder.readEncoder_ISR();
  }
}

void RotaryEncoder::onEncoderChanged(int value, int delta) {
  Serial.print("Value: ");
  Serial.print(value);

  if (delta > 0) {
    Serial.println(" (UP)");
  } else if (delta < 0) {
    Serial.println(" (DOWN)");
  }
}

void RotaryEncoder::onClick() {
  Serial.println("Button event: Single Click");
}

void RotaryEncoder::onDoubleClick() {
  Serial.println("Button event: Double Click");
}

void RotaryEncoder::onLongPressStart() {
  Serial.println("Button event: LongPress");
  switch (global_state) {
    case clock_config:
      global_state = clock_on;
      break;
    case clock_on:
      global_state = clock_config;
      break;
    default:
      global_state = clock_on;
      break;
  }
  Serial.print("New Global State is: ");
  Serial.println(global_state);
}