#include "WifiManagerWrapper.h"
#include "NTPServer.h"
#include "TM1637Wrapper.h"
#include <time.h>
#include "CSE_MillisTimer.h"


WifiManagerWrapper WifiWrapper;
NtpServer timeServer;
TM1637Wrapper Tm1637;
CSE_MillisTimer secondTimer(1000);  // Create a timer with a time period of 500 ms.

uint32_t time_to_resync = 030000;  // Formatted in HHmmss

tm current_time;  // Structure to hold the current time.

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  WifiWrapper.setup_wifi();
  timeServer.config_ntp();
  Tm1637.config_tm1637();

  pinMode(LED_BUILTIN, OUTPUT);

  current_time = timeServer.get_time();
  Tm1637.set_time(current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
  secondTimer.start(true);  // Start the timer and set the start state to true.
}

// the loop function runs over and over again forever
void loop() {
  if (current_time.tm_hour * 10000 + current_time.tm_min * 100 + current_time.tm_sec * 1 == time_to_resync) {
    current_time = timeServer.get_time();
  }

  if (secondTimer.isElapsed()) {
    secondTimer.start();
    Tm1637.set_time(current_time.tm_hour, current_time.tm_min, current_time.tm_sec);

    // Add one second
    time_t rawTime = mktime(&current_time);
    rawTime++;
    localtime_r(&rawTime, &current_time);
    Serial.println(current_time.tm_hour * 10000 + current_time.tm_min * 100 + current_time.tm_sec * 1);
  }
}
