#include "WifiManagerWrapper.h"
#include "NTPServer.h"
#include "TM1637Wrapper.h"
#include <time.h>
#include "CSE_MillisTimer.h"
#include "LedStrip.h"
#include "RotaryEncoder.h"
#include "GlobalVariables.h"


WifiManagerWrapper WifiWrapper;
NtpServer timeServer;
TM1637Wrapper ClockDisplay;
CSE_MillisTimer secondTimer(1000);  // Create a timer with a time period of 1000 ms.
LedStrip AlarmLeds;
RotaryEncoder Encoder;


constexpr uint32_t time_to_resync = 030000;  // Formatted in HHmmss

tm current_time;  // Structure to hold the current time.


GlobalStates global_state = clock_on;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  Serial.println("Beginning Clock");
  WifiWrapper.setup_wifi();
  timeServer.setup();
  ClockDisplay.setup();
  AlarmLeds.setup();
  Encoder.setup();

  pinMode(LED_BUILTIN, OUTPUT);

  current_time = timeServer.get_updated_time();
  ClockDisplay.set_time(current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
  secondTimer.start(true);  // Start the timer and set the start state to true.
}

// the loop function runs over and over again forever
void loop() {
  if (current_time.tm_hour * 10000 + current_time.tm_min * 100 + current_time.tm_sec * 1 == time_to_resync) {
    current_time = timeServer.get_updated_time();
  }

  if (secondTimer.isElapsed()) {
    secondTimer.start();
    ClockDisplay.set_time(current_time.tm_hour, current_time.tm_min, current_time.tm_sec);

    // Add one second
    time_t rawTime = mktime(&current_time);
    rawTime++;
    localtime_r(&rawTime, &current_time);
    Serial.println(current_time.tm_hour * 10000 + current_time.tm_min * 100 + current_time.tm_sec * 1);
  }
  AlarmLeds.write();
  Encoder.rotary_loop();
}
