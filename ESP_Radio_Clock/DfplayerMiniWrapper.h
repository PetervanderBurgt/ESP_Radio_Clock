#ifndef DFPLAYER_MINI_WRAPPER_H
#define DFPLAYER_MINI_WRAPPER_H

class DfPlayerMiniWrapper {
public:
  DfPlayerMiniWrapper();

  void setup();
  void loop();
  void playRandomAlarm();
  void stopAlarm();

  bool currentlyPlaying;
};

#endif