#ifndef FIRMWARE_INCLUDE_SONG_HPP
#define FIRMWARE_INCLUDE_SONG_HPP

#include "AudioGeneratorMP3.h"

#include "AudioOutputI2S.h"

class Song
{
    AudioFileSource* File;
    AudioGeneratorMP3* MP3;

    char* SongPath;

    Song* PreviousSong;
    Song* NextSong;

public:
    Song(const char* song_path);

    ~Song();

    [[nodiscard]] Song* getNextSong() const;
    [[nodiscard]] Song* getPreviousSong() const;

    void setPreviousSong(Song* song_ptr);
    void setNextSong(Song* song_ptr);

    bool isPlaying();

    void stop();
    void play(AudioOutputI2S* audio_output);

    [[nodiscard]] char* getSongPath() const;

    void freeBuffer();
};

#endif
