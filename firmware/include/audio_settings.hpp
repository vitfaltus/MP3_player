#ifndef FIRMWARE_INCLUDE_AUDIO_SETTINGS_HPP
#define FIRMWARE_INCLUDE_AUDIO_SETTINGS_HPP

#include "AudioOutputI2S.h"


class AudioSettings
{
    AudioOutputI2S* Amp;
    float realVolumeLevel = 0.3;
    float VirtualVolumeLevel;

    const float VolumeDiff = 0.05;

public:
    AudioSettings();
    ~AudioSettings();

    float volumeUp();

    void setVolume(float level);

    float volumeDown();

    void shutAudio();

    void restoreAudio();

    [[nodiscard]] float getVolume() const;

    [[nodiscard]] AudioOutputI2S* getAudioOutput() const;
};

#endif
