#include <Arduino.h>
#include "AudioOutputI2S.h"
#include "pin_config.hpp"
#include "audio_settings.hpp"


  AudioSettings::AudioSettings(){
    amp = new AudioOutputI2S();
    amp->SetPinout(PinConfig::I2S_BCLK, PinConfig::I2S_LRC, PinConfig::I2S_DOUT);
    amp->SetGain(m_real_volume_level);  // volume (0.0–1.0)
    m_virtual_volume_level = m_real_volume_level;
  }

  AudioSettings::~AudioSettings(){
    if (amp){
        delete amp;
    }
  }

  double AudioSettings::volume_up(){
    if (m_real_volume_level < 1.0){
      m_real_volume_level += VOLUME_DIFF;
      amp->SetGain(m_real_volume_level);
    }
    m_virtual_volume_level = m_real_volume_level;
    return m_real_volume_level;
  }

  void AudioSettings::set_volume(double level){
    m_real_volume_level = level;
    amp->SetGain(m_real_volume_level);
    m_virtual_volume_level = m_real_volume_level;
  }

  double AudioSettings::volume_down(){
    if (m_real_volume_level > 0.02){
      m_real_volume_level -= VOLUME_DIFF;
      amp->SetGain(m_real_volume_level);
    }
    m_virtual_volume_level = m_real_volume_level;
    return m_real_volume_level;
  }

  void AudioSettings::shut_audio(){
    m_real_volume_level = 0;
    amp->SetGain(m_real_volume_level);
  }

  void AudioSettings::restore_audio(){
    m_real_volume_level = m_virtual_volume_level;
    amp->SetGain(m_real_volume_level);
  }

  double AudioSettings::get_volume(){
    return m_real_volume_level;
  }

