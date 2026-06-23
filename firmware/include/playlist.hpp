#ifndef FIRMWARE_INCLUDE_PLAYLIST_HPP
#define FIRMWARE_INCLUDE_PLAYLIST_HPP

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
  void Play(const AudioSettings *audio);
  void Stop();
  [[nodiscard]] bool IsPaused() const;
  void PlaylistLoop(const AudioSettings *audio);
  void PlayNextSong(const AudioSettings *audio);
  void PlayPreviousSong(const AudioSettings *audio);
  void CreatePlaylist(File &current_dir);

  [[nodiscard]] char *GetSongName() const {
    return m_current_song->get_song_path();
  }
};


#endif