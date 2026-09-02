#include "WifiManagerWrapper.h"
#include "NTPServer.h"
#include "TM1637Wrapper.h"
#include <time.h>
#include "CSE_MillisTimer.h"
#include "LedStrip.h"
#include "RotaryEncoder.h"
#include "DfplayerMiniWrapper.h"
#include "GlobalVariables.h"


WifiManagerWrapper WifiWrapper;
NtpServer timeServer;
TM1637Wrapper ClockDisplay;
CSE_MillisTimer secondTimer(1000);  // Create a timer with a time period of 1000 ms.
LedStrip AlarmLeds;
RotaryEncoder Encoder;
DfPlayerMiniWrapper Dfplayer;


// constexpr CustomTime time_to_resync{03, 00, 00};  // Formatted in HH, mm, ss
constexpr CustomTime time_to_resync{ 17, 31, 00 };  // Formatted in HH, mm, ss

tm current_time;  // Structure to hold the current time.
extern tm config_time;

CustomTime alarm_times[7] = { 0, 0, 0 };  // Formatted in HHmmss

bool snoozeRequested = false;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  Serial.println("Beginning Clock");
  WifiWrapper.setup();
  timeServer.setup();
  Encoder.setup();
  ClockDisplay.setup();
  AlarmLeds.setup();
  Dfplayer.setup();

  pinMode(LED_BUILTIN, OUTPUT);

  current_time = timeServer.get_updated_time();
  ClockDisplay.set_time(current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
  secondTimer.start(true);  // Start the timer and set the start state to true.
}

// the loop function runs over and over again forever
void loop() {
  if (current_time.tm_hour == time_to_resync.hours && current_time.tm_min == time_to_resync.minutes && current_time.tm_sec == time_to_resync.seconds) {
    current_time = timeServer.get_updated_time();
  }

  uint8_t weekday = current_time.tm_wday;  // 0 = Sunday ... 6 = Saturday
  uint8_t weekday_monday_first = (weekday + 6) % 7;
  // Serial.print(alarm_times[weekday_monday_first].hours);
  // Serial.print(":");
  // Serial.print(alarm_times[weekday_monday_first].minutes);
  // Serial.print(":");
  // Serial.println(alarm_times[weekday_monday_first].seconds);

  if (current_time.tm_hour == alarm_times[weekday_monday_first].hours && current_time.tm_min == alarm_times[weekday_monday_first].minutes && current_time.tm_sec == alarm_times[weekday_monday_first].seconds) {
    //Turn Alarm on
    Serial.println("Alarm should go off here");
    Dfplayer.playRandomAlarm();
  }

  if (snoozeRequested) {
    //Turn Alarm off
    Serial.println("Alarm should be turned off");
    Dfplayer.stopAlarm();
    snoozeRequested = false;
  }

  if (secondTimer.isElapsed()) {
    secondTimer.start();
    // Add one second
    time_t rawTime = mktime(&current_time);
    rawTime++;
    localtime_r(&rawTime, &current_time);
    Serial.println(current_time.tm_hour * 10000 + current_time.tm_min * 100 + current_time.tm_sec * 1);
  }

  ClockDisplay.loop(current_time, config_time);
  AlarmLeds.loop();
  Encoder.rotary_loop();
  Dfplayer.loop();
}
