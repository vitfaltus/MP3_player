#include "audio_settings.hpp"

#include "song.hpp"
#include <SD.h>

class Playlist{

    char* m_root_path;

    Song* m_current_song;
    bool m_paused;

    char* add_dir_slash(const char* s);
    void CreateSequentialPlaylist(const char* path);

    public:

    Playlist(const char* path);
    ~Playlist();
    
    void AddSong(const char* path);
    void Play(AudioSettings* audio);
    void Stop();
    bool IsPaused();
    void PlaylistLoop();
    void PlayNextSong();
    void PlayPreviousSong();
    void CreatePlaylist(File& current_dir);
};