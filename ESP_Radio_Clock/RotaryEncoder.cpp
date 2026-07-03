#include "RotaryEncoder.h"
#include "AiEsp32RotaryEncoder.h"
#include <OneButton.h>
#include <time.h>

#define ROTARY_ENCODER_A_PIN D6
#define ROTARY_ENCODER_B_PIN D5
#define ROTARY_ENCODER_BUTTON_PIN D7
#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */


//depending on your encoder - try 1,2 or 4 to get expected behaviour
// #define ROTARY_ENCODER_STEPS 1
// #define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4

//set boundaries and if values should cycle or not
#define MIN_ENCODER_VALUE_MINUTE 0
#define MAX_ENCODER_VALUE_MINUTE 59
#define MIN_ENCODER_VALUE_HOUR 0
#define MAX_ENCODER_VALUE_HOUR 23
#define MIN_ENCODER_VALUE_OFF 0
#define MAX_ENCODER_VALUE_ON 1
#define LOOP_BACK_VALUE false

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
ConfigStates config_state = config_alarm_monday_hour;
bool alarm_on[7] = { false };

extern CustomTime alarm_times[7];

extern bool snoozeRequested;


tm config_time = { 0 };  // Structure to hold the config time.

RotaryEncoder* RotaryEncoder::instance = nullptr;

RotaryEncoder::RotaryEncoder()
  : aiRotaryEncoder(ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_A_PIN, -1, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS),
    button(ROTARY_ENCODER_BUTTON_PIN, true) {
  instance = this;
  lastEncoderValue = 0;
}

void RotaryEncoder::setup() {
  //we must initialize rotary encoder
  aiRotaryEncoder.begin();
  aiRotaryEncoder.setup(readEncoderISR);
  aiRotaryEncoder.setBoundaries(MIN_ENCODER_VALUE_HOUR, MAX_ENCODER_VALUE_HOUR, LOOP_BACK_VALUE);  //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

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
  int day_number = int(config_state / 3);

  if (config_state % 3 == 0) {
    config_time.tm_hour = value;
    alarm_times[day_number].hours = config_time.tm_hour;
  } else if (config_state % 3 == 1) {
    config_time.tm_min = value;
    alarm_times[day_number].minutes = config_time.tm_min;
  } else if (config_state % 3 == 2) {
    config_time.tm_sec = value;
    alarm_on[day_number] = bool(value);
    Serial.print(alarm_times[day_number].hours);
    Serial.print(":");
    Serial.println(alarm_times[day_number].minutes);
  }


  if (delta > 0) {
    Serial.println(" (UP)");
  } else if (delta < 0) {
    Serial.println(" (DOWN)");
  }
}

void RotaryEncoder::onClick() {
  Serial.println("Button event: Single Click");
  switch (global_state) {
    case clock_config:
      config_state = static_cast<ConfigStates>((config_state + 1) % config_alarm_count);
      if (instance) {
        if (config_state % 3 == 0) {
          instance->aiRotaryEncoder.setBoundaries(MIN_ENCODER_VALUE_HOUR, MAX_ENCODER_VALUE_HOUR, LOOP_BACK_VALUE);
          //Reset to 0
          instance->aiRotaryEncoder.setEncoderValue(0);
        } else if (config_state % 3 == 1) {
          instance->aiRotaryEncoder.setBoundaries(MIN_ENCODER_VALUE_MINUTE, MAX_ENCODER_VALUE_MINUTE, LOOP_BACK_VALUE);
          //Reset to 0
          instance->aiRotaryEncoder.setEncoderValue(0);
        } else if (config_state % 3 == 2) {
          instance->aiRotaryEncoder.setBoundaries(MIN_ENCODER_VALUE_OFF, MAX_ENCODER_VALUE_ON, LOOP_BACK_VALUE);
          //Reset to 0
          instance->aiRotaryEncoder.setEncoderValue(0);
        }
      }
      break;
    case clock_on:
      snoozeRequested = true;
      break;
    default:
      break;
  }

  Serial.print("New Config State is: ");
  Serial.println(config_state);
}

void RotaryEncoder::onDoubleClick() {
  Serial.println("Button event: Double Click");
  switch (global_state) {
    case clock_config:
      break;
    case clock_on:
      break;
    default:
      break;
  }
}

void RotaryEncoder::onLongPressStart() {
  Serial.println("Button event: LongPress");
  switch (global_state) {
    case clock_config:
      global_state = clock_on;
      break;
    case clock_on:
      global_state = clock_config;
      config_state = config_alarm_monday_hour;
      break;
    default:
      global_state = clock_on;
      break;
  }
  Serial.print("New Global State is: ");
  Serial.println(global_state);
}