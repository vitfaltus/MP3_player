#ifndef FIRMWARE_INCLUDE_SONGPLAYER_HPP
#define FIRMWARE_INCLUDE_SONGPLAYER_HPP

#include "audio_settings.hpp"
#include "song.hpp"

#include <SD.h>

class SongPlayer
{

    char* RootPath;

    Song* RootSong;

    Song* CurrentSong;
    bool Paused;

    static char* addDirSlash(const char* s);
    static bool isMP3File(const char* s);

    void createSequentialPlaylist(const char* path);

public:
    explicit SongPlayer(const char* path);
    ~SongPlayer();

    void addSong(const char* path);
    void play(const AudioSettings* audio);
    void stop();
    [[nodiscard]] bool isPaused() const;
    bool playerLoop(const AudioSettings* audio);
    void playNextSong(const AudioSettings* audio);
    void playPreviousSong(const AudioSettings* audio);
    void createPlaylist(File& current_dir);

    void rollToSong(const char* song_path);

    // false -> data in names invalid
    bool getThreeSongNames(unsigned song_start_position, std::array<char*, 3>& DisplaySongArr) const;

    [[nodiscard]] char* getSongName() const;

};


#endif
