#include "audio_settings.hpp"
#include "AudioOutputI2S.h"
#include "pin_config.hpp"

AudioSettings::AudioSettings()
{
    Amp = new AudioOutputI2S();
    Amp->SetPinout(PinConfig::I2S_BCLK, PinConfig::I2S_LRC,
                   PinConfig::I2S_DOUT);
    Amp->SetGain(VolumeLevel); // volume (0.0–1.0)
}

AudioSettings::~AudioSettings() { delete Amp; }

float AudioSettings::volumeUp()
{
    if (VolumeLevel < 1.0)
    {
        VolumeLevel += VolumeDiff;
        Amp->SetGain(VolumeLevel);
    }
    return VolumeLevel;
}

void AudioSettings::setVolume(const float level)
{
    VolumeLevel = level;
    Amp->SetGain(VolumeLevel);
}

float AudioSettings::volumeDown()
{
    if (VolumeLevel > 0.02)
    {
        VolumeLevel -= VolumeDiff;
        Amp->SetGain(VolumeLevel);
    }
    return VolumeLevel;
}

void AudioSettings::shutAudio() const
{
    Amp->SetGain(0);
}

void AudioSettings::restoreAudio() const
{
    Amp->SetGain(VolumeLevel);

}

float AudioSettings::getVolume() const { return VolumeLevel; }

AudioOutputI2S* AudioSettings::getAudioOutput() const { return Amp; }
