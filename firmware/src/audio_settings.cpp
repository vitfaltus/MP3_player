#include <Arduino.h>
#include "AudioOutputI2S.h"
#include "pin_config.hpp"


class AudioSettings {
  AudioOutputI2S* amp;
  double volume_level = 0.3;
  const double VOLUME_DIFF = 0.05;

  public:

  AudioSettings(){
    amp = new AudioOutputI2S();
    amp->SetPinout(PinConfig::I2S_BCLK, PinConfig::I2S_LRC, PinConfig::I2S_DOUT);
    amp->SetGain(volume_level);  // volume (0.0–1.0)
  }

  ~AudioSettings(){
    if (amp){
        delete amp;
    }
  }

  double volume_up(){
    if (volume_level < 1.0){
      volume_level += VOLUME_DIFF;
      amp->SetGain(volume_level);
    }
    return volume_level;
  }

  void set_volume(double level){
    volume_level = level;
    amp->SetGain(volume_level);
  }

  double volume_down(){
    if (volume_level > 0.02){
      volume_level -= VOLUME_DIFF;
      amp->SetGain(volume_level);
    }
    return volume_level;
  }

  double get_volume(){
    return volume_level;
  }

};
