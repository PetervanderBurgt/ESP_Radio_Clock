#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

typedef void (*CallbackFunction)();
typedef void (*EncoderCallback)(int delta);

enum GlobalStates {
  clock_on,
  clock_config
};


#endif