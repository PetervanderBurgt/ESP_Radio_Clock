#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

typedef void (*CallbackFunction)();
typedef void (*EncoderCallback)(int delta);

enum GlobalStates {
  clock_on,
  clock_config
};

enum ConfigStates {
  config_alarm_monday_hour,
  config_alarm_monday_minute,
  config_alarm_monday_alarm,
  config_alarm_tuesday_hour,
  config_alarm_tuesday_minute,
  config_alarm_tuesday_alarm,
  config_alarm_wednesday_hour,
  config_alarm_wednesday_minute,
  config_alarm_wednesday_alarm,
  config_alarm_thursday_hour,
  config_alarm_thursday_minute,
  config_alarm_thursday_alarm,
  config_alarm_friday_hour,
  config_alarm_friday_minute,
  config_alarm_friday_alarm,
  config_alarm_saturday_hour,
  config_alarm_saturday_minute,
  config_alarm_saturday_alarm,
  config_alarm_sunday_hour,
  config_alarm_sunday_minute,
  config_alarm_sunday_alarm,
  config_alarm_count
};


#endif