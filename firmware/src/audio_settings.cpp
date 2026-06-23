#include "audio_settings.hpp"
#include "AudioOutputI2S.h"
#include "pin_config.hpp"

AudioSettings::AudioSettings()
{
    Amp = new AudioOutputI2S();
    Amp->SetPinout(PinConfig::I2S_BCLK, PinConfig::I2S_LRC,
                   PinConfig::I2S_DOUT);
    Amp->SetGain(realVolumeLevel); // volume (0.0–1.0)
    VirtualVolumeLevel = realVolumeLevel;
}

AudioSettings::~AudioSettings() { delete Amp; }

float AudioSettings::volumeUp()
{
    if (realVolumeLevel < 1.0)
    {
        realVolumeLevel += VolumeDiff;
        Amp->SetGain(realVolumeLevel);
    }
    VirtualVolumeLevel = realVolumeLevel;
    return realVolumeLevel;
}

void AudioSettings::setVolume(float level)
{
    realVolumeLevel = level;
    Amp->SetGain(realVolumeLevel);
    VirtualVolumeLevel = realVolumeLevel;
}

float AudioSettings::volumeDown()
{
    if (realVolumeLevel > 0.02)
    {
        realVolumeLevel -= VolumeDiff;
        Amp->SetGain(realVolumeLevel);
    }
    VirtualVolumeLevel = realVolumeLevel;
    return realVolumeLevel;
}

void AudioSettings::shutAudio()
{
    realVolumeLevel = 0;
    Amp->SetGain(realVolumeLevel);
}

void AudioSettings::restoreAudio()
{
    realVolumeLevel = VirtualVolumeLevel;
    Amp->SetGain(realVolumeLevel);
}

float AudioSettings::getVolume() const { return realVolumeLevel; }

AudioOutputI2S* AudioSettings::getAudioOutput() const { return Amp; }
