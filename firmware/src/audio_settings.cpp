#include "audio_settings.hpp"
#include "AudioOutputI2S.h"
#include "config.hpp"

C_AudioSettings::C_AudioSettings()
{
    Amp = new AudioOutputI2S();
    Amp->SetPinout(config::PinConfig::I2S_BCLK, config::PinConfig::I2S_LRC,
                   config::PinConfig::I2S_DOUT);
    Amp->SetGain(VolumeLevel); // volume (0.0–1.0)
}

C_AudioSettings::~C_AudioSettings() { delete Amp; }

float C_AudioSettings::volumeUp()
{
    if (VolumeLevel < 1.0)
    {
        VolumeLevel += VolumeDiff;
        Amp->SetGain(VolumeLevel);
    }
    return VolumeLevel;
}

void C_AudioSettings::setVolume(const float level)
{
    VolumeLevel = level;
    Amp->SetGain(VolumeLevel);
}

float C_AudioSettings::volumeDown()
{
    if (VolumeLevel > 0.02)
    {
        VolumeLevel -= VolumeDiff;
        Amp->SetGain(VolumeLevel);
    }
    return VolumeLevel;
}

void C_AudioSettings::shutAudio() const
{
    Amp->SetGain(0);
}

void C_AudioSettings::restoreAudio() const
{
    Amp->SetGain(VolumeLevel);

}

float C_AudioSettings::getVolume() const { return VolumeLevel; }

AudioOutputI2S* C_AudioSettings::getAudioOutput() const { return Amp; }
