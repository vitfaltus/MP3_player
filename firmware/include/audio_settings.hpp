#ifndef FIRMWARE_INCLUDE_AUDIO_SETTINGS_HPP
#define FIRMWARE_INCLUDE_AUDIO_SETTINGS_HPP

#include "AudioOutputI2S.h"


class C_AudioSettings
{
    AudioOutputI2S* Amp;
    float VolumeLevel = 0.3;

    const float VolumeDiff = 0.05;

public:
    C_AudioSettings();
    ~C_AudioSettings();

    float volumeUp();

    void setVolume(float level);

    float volumeDown();

    void shutAudio() const;

    void restoreAudio() const;

    [[nodiscard]] float getVolume() const;

    [[nodiscard]] AudioOutputI2S* getAudioOutput() const;
};

#endif
