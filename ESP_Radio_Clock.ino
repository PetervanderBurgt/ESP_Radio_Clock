#include "WifiManagerWrapper.h"
#include "NTPServer.h"

WifiManagerWrapper WifiWrapper;
NtpServer timeServer;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  WifiWrapper.setup_wifi();
  timeServer.config_ntp();


  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // change state of the LED by setting the pin to the HIGH voltage level
  Serial.println("LED is on");
  delay(1000);                     // wait for a second
  digitalWrite(LED_BUILTIN, LOW);  // change state of the LED by setting the pin to the LOW voltage level
  Serial.println("LED is off");
  delay(1000);  // wait for a second
  timeServer.get_time();
}
