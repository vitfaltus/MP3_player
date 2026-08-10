#include "song.hpp"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

Song::Song(const char* song_path)
{
    SongPath = strdup(song_path);
    MP3 = nullptr;
    File = nullptr;
    PreviousSong = nullptr;
    NextSong = nullptr;
}

Song::~Song()
{
    freeBuffer();

    if (SongPath)
    {
        free(SongPath);
    }

    if (NextSong)
    {
        NextSong->setPreviousSong(nullptr);
    }
    if (PreviousSong)
    {
        PreviousSong->setPreviousSong(nullptr);
    }
}

Song* Song::getNextSong() const { return NextSong; }

Song* Song::getPreviousSong() const { return PreviousSong; }

void Song::setPreviousSong(Song* song_ptr) { PreviousSong = song_ptr; }

void Song::setNextSong(Song* song_ptr) { NextSong = song_ptr; }

bool Song::isPlaying()
{
    if (MP3->isRunning())
    {
        if (!MP3->loop())
        {
            MP3->stop();
            freeBuffer();
            return false;
        }
        return true;
    }
    return false;
}

void Song::stop() { freeBuffer(); }

void Song::play(AudioOutputI2S* audio_output)
{
    File = new AudioFileSourceSD(SongPath);
    MP3 = new AudioGeneratorMP3();
    Serial.println("Started playing");
    MP3->begin(File, audio_output);
}

char* Song::getSongPath() const { return SongPath; }

void Song::freeBuffer()
{
    if (File)
    {
        File->close();
        delete File;
        File = nullptr;
    }
    if (MP3)
    {
        MP3->stop();
        delete MP3;
        MP3 = nullptr;
    }
}
