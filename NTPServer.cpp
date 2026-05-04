#include "NTPServer.h"
#include <ESP8266WiFi.h>
#include <time.h>  // for time() ctime()

/* Configuration of NTP */
#define MY_NTP_SERVER "at.pool.ntp.org"           
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"

/* Globals */
time_t now;  // this are the seconds since Epoch (1970) - UTC
tm tm;       // the structure tm holds time information in a more convenient way


void NtpServer::config_ntp() {
  configTime(MY_TZ, MY_NTP_SERVER); // --> Here is the IMPORTANT ONE LINER needed in your sketch!
}

void NtpServer::get_time() {
  time(&now);              // read the current time
  localtime_r(&now, &tm);  // update the structure tm with the current time
  Serial.print("year:");
  Serial.print(tm.tm_year + 1900);  // years since 1900
  Serial.print("\tmonth:");
  Serial.print(tm.tm_mon + 1);  // January = 0 (!)
  Serial.print("\tday:");
  Serial.print(tm.tm_mday);  // day of month
  Serial.print("\thour:");
  Serial.print(tm.tm_hour);  // hours since midnight  0-23
  Serial.print("\tmin:");
  Serial.print(tm.tm_min);  // minutes after the hour  0-59
  Serial.print("\tsec:");
  Serial.print(tm.tm_sec);  // seconds after the minute  0-61*
  Serial.print("\twday");
  Serial.print(tm.tm_wday);  // days since Sunday 0-6
  if (tm.tm_isdst == 1)      // Daylight Saving Time flag
    Serial.print("\tDST");
  else
    Serial.print("\tstandard");
  Serial.println();
}