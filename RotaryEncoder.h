#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

class RotaryEncoder {
public:
    void setup();
    void rotary_loop();
    static void readEncoderISR();
};

#endif