#ifndef TM1637_WRAPPER_H
#define TM1637_WRAPPER_H
#include <stdint.h>
#include <time.h>

class TM1637Wrapper {
public:
    void setup();
    void set_time(uint8_t hour, uint8_t minutes, uint8_t seconds);
    void loop(tm current_time, tm config_time);
    void updateBrightness();
};

#endif