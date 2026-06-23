#ifndef FIRMWARE_INCLUDE_PLAYLIST_HPP
#define FIRMWARE_INCLUDE_PLAYLIST_HPP

#include "audio_settings.hpp"
#include "song.hpp"

#include <SD.h>

class Playlist
{

    char* RootPath;

    Song* CurrentSong;
    bool Paused;

    static char* addDirSlash(const char* s);
    void createSequentialPlaylist(const char* path);

public:
    explicit Playlist(const char* path);
    ~Playlist();

    void addSong(const char* path);
    void play(const AudioSettings* audio);
    void stop();
    [[nodiscard]] bool isPaused() const;
    void playlistLoop(const AudioSettings* audio);
    void playNextSong(const AudioSettings* audio);
    void playPreviousSong(const AudioSettings* audio);
    void createPlaylist(File& current_dir);

    [[nodiscard]] char* getSongName() const
    {
        return CurrentSong->getSongPath();
    }
};


#endif
