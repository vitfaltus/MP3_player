#include "playlist.hpp"
#include <SD.h>
#include "audio_settings.hpp"

char* Playlist::addDirSlash(const char* s)
{
    const size_t len = strlen(s);

    const auto out = static_cast<char*>(malloc(len + 2));
    if (!out)
    {
        Serial.println("Path name not allocated");
        return nullptr;
    }

    out[0] = '/';
    memcpy(out + 1, s, len + 1);

    return out;
}

void Playlist::createSequentialPlaylist(const char* path)
{
    RootPath = strdup(path);
    File root_path_file = SD.open(path);
    createPlaylist(root_path_file);
    Serial.println("Playlist created");
}

Playlist::Playlist(const char* path)
{
    CurrentSong = nullptr;
    RootPath = nullptr;
    this->createSequentialPlaylist(path);
    Paused = true;
}

Playlist::~Playlist()
{
    if (!CurrentSong)
    {
        return;
    }
    while (CurrentSong->getPreviousSong())
    {
        CurrentSong = CurrentSong->getNextSong();
    }
    Song* next_song = CurrentSong->getNextSong();
    while (CurrentSong)
    {
        CurrentSong->~Song();
        CurrentSong = next_song;
        next_song = CurrentSong->getNextSong();
    }
}

void Playlist::addSong(const char* path)
{
    const char* slash_name = addDirSlash(path);
    if (!slash_name)
    {
        return;
    }

    Song* new_song = new Song(slash_name);

    if (CurrentSong == nullptr)
    {
        CurrentSong = new_song;
    }

    else
    {

        Song* tmp = CurrentSong;

        while (tmp->getNextSong() != nullptr)
        {
            tmp = tmp->getNextSong();
        }

        tmp->setNextSong(new_song);
        new_song->setPreviousSong(tmp);
    }
}

void Playlist::play(const AudioSettings* audio)
{
    Paused = false;
    CurrentSong->play(audio->getAudioOutput());
}

void Playlist::stop()
{
    CurrentSong->stop();
    Paused = true;
}

bool Playlist::isPaused() const { return Paused; }

bool Playlist::playlistLoop(const AudioSettings* audio)
{
    if (!Paused)
    {
        if (!CurrentSong->isPlaying())
        {
            playNextSong(audio);
            return true;
        }
    }
    return false;
}

void Playlist::playNextSong(const AudioSettings* audio)
{

    if (Song* next_song = CurrentSong->getNextSong())
    {

        CurrentSong->freeBuffer();

        CurrentSong = next_song;
        if (!Paused)
        {
            CurrentSong->play(audio->getAudioOutput());
        }

        Serial.print("Now playing ");
        Serial.println(CurrentSong->getSongPath());
    }
    else
    {
        Serial.println("No next song");
    }
}

void Playlist::playPreviousSong(const AudioSettings* audio)
{

    if (Song* previous_song = CurrentSong->getPreviousSong())
    {

        CurrentSong->freeBuffer();

        CurrentSong = previous_song;
        if (!Paused)
        {
            CurrentSong->play(audio->getAudioOutput());
        }

        Serial.print("Now playing ");
        Serial.println(CurrentSong->getSongPath());
    }
    else
    {
        Serial.println("No previous song");
    }
}

void Playlist::createPlaylist(File& current_dir)
{
    while (true)
    {
        File entry = current_dir.openNextFile();
        if (!entry)
        {
            break;
        }
        if (!entry.isDirectory())
        {
            this->addSong(entry.name());
        }
    }
}
