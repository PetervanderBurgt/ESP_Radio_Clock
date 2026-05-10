#include "NTPServer.h"
#include <ESP8266WiFi.h>
#include <time.h>  // for time() ctime()

/* Configuration of NTP */
#define MY_NTP_SERVER "at.pool.ntp.org"
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"

/* Globals */
time_t now;   // this are the seconds since Epoch (1970) - UTC
tm time_var;  // the structure time_var holds time information in a more convenient way

uint8_t max_time_sync_seconds = 120;

void NtpServer::config_ntp() {
  configTime(MY_TZ, MY_NTP_SERVER);  // --> Here is the IMPORTANT ONE LINER needed in your sketch!

  // Wait for the time sync for a max of 100 seconds
  time_var = get_time();
  while (mktime(&time_var) < (max_time_sync_seconds * 1000)) {
    delay(100);
    time_var = get_time();
    Serial.println("Waiting for NTP sync...");
  }
}

tm NtpServer::get_time() {
  time(&now);                    // read the current time
  localtime_r(&now, &time_var);  // update the structure time_var with the current time
  Serial.print("year:");
  Serial.print(time_var.tm_year + 1900);  // years since 1900
  Serial.print("\timeonth:");
  Serial.print(time_var.tm_mon + 1);  // January = 0 (!)
  Serial.print("\tday:");
  Serial.print(time_var.tm_mday);  // day of month
  Serial.print("\thour:");
  Serial.print(time_var.tm_hour);  // hours since midnight  0-23
  Serial.print("\tmin:");
  Serial.print(time_var.tm_min);  // minutes after the hour  0-59
  Serial.print("\tsec:");
  Serial.print(time_var.tm_sec);  // seconds after the minute  0-61*
  Serial.print("\twday");
  Serial.print(time_var.tm_wday);  // days since Sunday 0-6
  if (time_var.tm_isdst == 1)      // Daylight Saving Time flag
    Serial.print("\tDST");
  else
    Serial.print("\tstandard");
  Serial.println();
  return time_var;
}