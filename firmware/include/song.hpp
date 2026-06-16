#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"

#include "AudioOutputI2S.h"

class Song {
  AudioFileSource *m_file;
  AudioGeneratorMP3 *m_mp3;

  char *m_song_path;

  Song *m_previous_song;
  Song *m_next_song;

public:
  Song(const char *song_path);

  ~Song();

  Song *get_next_song();
  Song *get_previous_song();

  void set_previous_song(Song *song_ptr);
  void set_next_song(Song *song_ptr);

  bool is_playing();

  void stop();
  void play(AudioOutputI2S *audio_output);

  char *get_song_path();

  void free_buffer();
};