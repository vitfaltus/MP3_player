#pragma once
#include "AudioOutputI2S.h"

#pragma once

class AudioSettings {
  AudioOutputI2S *amp;
  float m_real_volume_level = 0.3;
  float m_virtual_volume_level;

  const float VOLUME_DIFF = 0.05;

public:
  AudioSettings();
  ~AudioSettings();

  float volume_up();

  void set_volume(float level);

  float volume_down();

  void shut_audio();

  void restore_audio();

  [[nodiscard]] float get_volume() const;

  [[nodiscard]] AudioOutputI2S *get_audio_output() const;
};
