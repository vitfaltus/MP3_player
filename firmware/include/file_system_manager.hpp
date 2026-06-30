#include "AudioFileSourceFS.h"

#define CURRENT_SONG_FILE "/.current_song_path"


class FileSystemManager {
  bool FileSystemPresent = false;

  static bool setup();

  public:
  FileSystemManager();

  bool getCurrentSongPath(char* path) const;
  bool setCurrentSongPath(const char* path) const;




};

