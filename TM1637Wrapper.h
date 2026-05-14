#ifndef TM1637_WRAPPER_H
#define TM1637_WRAPPER_H
#include <stdint.h>

class TM1637Wrapper {
public:
    void setup();
    void set_time(uint8_t hour, uint8_t minutes, uint8_t seconds);
};

#endif