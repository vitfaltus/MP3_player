#include "song.hpp"
#include <SD.h>

class Playlist{

    char* m_root_path;

    Song* m_current_song;
    bool m_paused;

    char* add_dir_slash(const char* s);

    public:

    Playlist(const char* root_path);
    ~Playlist();
    void AddSong(const char* path);
    void Play();
    void Stop();
    bool IsPaused();
    void PlaylistLoop();
    void PlayNextSong();
    void PlayPreviousSong();
    void CreatePlaylist(File& current_dir);
};