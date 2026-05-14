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
//rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
//or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
#define ROTARY_ENCODER_ACCELERATION 0


#define SINGLE_BUTTON_CLICK_MS 250
#define LONG_BUTTON_CLICK_MS 500


//instead of changing here, rather change numbers above
// Set Button to -1 as this will be handled by OneButton
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, -1, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
OneButton button(ROTARY_ENCODER_BUTTON_PIN, true);

void RotaryEncoder::setup() {
  //we must initialize rotary encoder
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(MIN_ENCODER_VALUE, MAX_ENCODER_VALUE, LOOP_BACK_VALUE);  //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

  rotaryEncoder.setAcceleration(ROTARY_ENCODER_ACCELERATION);

  button.setClickMs(SINGLE_BUTTON_CLICK_MS);
  button.setPressMs(LONG_BUTTON_CLICK_MS);
  button.attachClick([]() {
    Serial.println("Single Click");
  });

  button.attachDoubleClick([]() {
    Serial.println("Double Click");
  });

  button.attachLongPressStart([]() {
    Serial.println("Long Press");
  });
}

void RotaryEncoder::rotary_loop() {
  //dont print anything unless value changed
  if (rotaryEncoder.encoderChanged()) {
    Serial.print("Value: ");
    Serial.println(rotaryEncoder.readEncoder());
  }
  button.tick();
}

void IRAM_ATTR RotaryEncoder::readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}