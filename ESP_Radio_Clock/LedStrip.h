#ifndef LEDSTRIP_H
#define LEDSTRIP_H

class LedStrip {
public:
  void setup();
  void loop();
  void updateBrightness();


private:
  void setAlarm();
};

#endif