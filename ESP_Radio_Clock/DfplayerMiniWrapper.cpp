#include <Arduino.h>
#include <SoftwareSerial.h>
#include "DfPlayerMiniWrapper.h"
#include <DFMiniMp3.h>

// forward declare the notify class, just the name
//
class Mp3Notify;

// Some arduino boards only have one hardware serial port, so a software serial port is needed instead.
// comment out the above definitions and use these
SoftwareSerial secondarySerial(5, 4);  // RX, TX
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
DfMp3 dfmp3(secondarySerial);

// implement a notification class,
// its member methods will get called
//
class Mp3Notify {
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
    if (source & DfMp3_PlaySources_Sd) {
      Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) {
      Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) {
      Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode) {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
    Serial.print("Play finished for #");
    Serial.println(track);
  }
  static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "removed");
  }
};

DfPlayerMiniWrapper::DfPlayerMiniWrapper() {
  currentlyPlaying = false;
}


void DfPlayerMiniWrapper::setup() {
  Serial.println("initializing DfPlayer Mini");
  secondarySerial.begin(9600);
  dfmp3.begin();
  // for boards that support hardware arbitrary pins
  // dfmp3.begin(10, 11); // RX, TX

  // during development, it's a good practice to put the module
  // into a known state by calling reset().
  // You may hear popping when starting and you can remove this
  // call to reset() once your project is finalized
  //   dfmp3.reset();

  uint16_t version = dfmp3.getSoftwareVersion();
  Serial.print("version ");
  Serial.println(version);

  uint16_t volume = dfmp3.getVolume();
  Serial.print("volume ");
  Serial.println(volume);
  dfmp3.setVolume(30);

  uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  Serial.print("files ");
  Serial.println(count);

  dfmp3.stop();
}

void DfPlayerMiniWrapper::loop() {
  dfmp3.loop();
}

void DfPlayerMiniWrapper::playRandomAlarm() {
  if (!currentlyPlaying) {
    uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    uint8_t MAX_ATTEMPTS = 10;
    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
      uint16_t track = random(1, count + 1);

      Serial.print("Trying track ");
      Serial.println(track);

      dfmp3.stop();
      delay(100);

      dfmp3.loopGlobalTrack(track);

      delay(300);
      dfmp3.loop();

      DfMp3_Status status = dfmp3.getStatus();

      if (status.state == DfMp3_StatusState_Playing) {
        currentlyPlaying = true;
        return;
      }
    }

    Serial.println("Failed to start playback");
    currentlyPlaying = false;
  }
}

void DfPlayerMiniWrapper::stopAlarm() {
  if (!currentlyPlaying) {
    return;
  }

  const uint8_t MAX_ATTEMPTS = 10;

  for (uint8_t attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
    dfmp3.stop();

    delay(100);
    dfmp3.loop();

    DfMp3_Status status = dfmp3.getStatus();

    if (status.state != DfMp3_StatusState_Playing) {
      currentlyPlaying = false;
      Serial.println("Playback stopped.");
      return;
    }

    Serial.print("Stop attempt ");
    Serial.print(attempt + 1);
    Serial.println(" failed.");
  }

  Serial.println("Failed to stop playback.");
}