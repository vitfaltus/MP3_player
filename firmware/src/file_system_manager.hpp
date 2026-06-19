#include "AudioFileSourceFS.h"
class FileSystemManager {
  bool file_system_present = false;

  static bool filesys_setup();

  public:
  FileSystemManager();

  bool get_current_song_path(std::vector<char>& path) const;
  bool set_current_song_path(const std::vector<char>& path) const;




};

