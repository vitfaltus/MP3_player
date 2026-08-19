#ifndef FIRMWARE_INCLUDE_SONGPLAYER_HPP
#define FIRMWARE_INCLUDE_SONGPLAYER_HPP

#include "audio_settings.hpp"
#include "song.hpp"

#include <SD.h>

class C_SongPlayer
{

    char* RootPath;

    Song* RootSong;

    Song* CurrentSong;
    bool Paused;

    static char* addDirSlash(const char* s);
    static bool isMP3File(const char* s);

    void createSequentialPlaylist(const char* path);

public:
    explicit C_SongPlayer(const char* path);
    ~C_SongPlayer();

    void addSong(const char* path);
    void play(const C_AudioSettings* audio);
    void stop();
    [[nodiscard]] bool isPaused() const;
    bool playerLoop(const C_AudioSettings* audio);
    void playNextSong(const C_AudioSettings* audio);
    void playPreviousSong(const C_AudioSettings* audio);
    void createPlaylist(File& current_dir);

    void rollToSong(const char* song_path);

    // false -> data in names invalid
    bool getThreeSongNames(unsigned song_start_position, std::array<char*, 3>& DisplaySongArr) const;

    [[nodiscard]] char* getSongName() const;

};


#endif
