#include "AudioFileSourceFS.h"

#define CURRENT_SONG_FILE "/.current_song_path"


class FileSystemManager {
  bool FileSystemPresent = false;

  static bool setup();

  public:
  FileSystemManager();

  bool getCurrentSongPath(std::vector<char>& path) const;
  bool setCurrentSongPath(const std::vector<char>& path) const;




};

