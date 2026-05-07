#include "song.hpp"

class Playlist{

    Song* m_current_song;
    bool m_paused;

    char* add_dir_slash();

    public:

    Playlist();
    ~Playlist();
    void AddSong(const char* path);
    void Play();
    void Stop();
    void PlaylistLoop();
    void PlayNextSong();
    void PlayPreviousSong();

};