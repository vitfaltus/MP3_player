#include "audio_settings.hpp"

#include "song.hpp"
#include <SD.h>

class Playlist {

  char *m_root_path;

  Song *m_current_song;
  bool m_paused;

  static char *add_dir_slash(const char *s);
  void CreateSequentialPlaylist(const char *path);

public:
  Playlist(const char *path);
  ~Playlist();

  void AddSong(const char *path);
  void Play(AudioSettings *audio);
  void Stop();
  bool IsPaused() const;
  void PlaylistLoop(AudioSettings *audio);
  void PlayNextSong(AudioSettings *audio);
  void PlayPreviousSong(AudioSettings *audio);
  void CreatePlaylist(File &current_dir);

  char *GetSongName() { return m_current_song->get_song_path(); }
};