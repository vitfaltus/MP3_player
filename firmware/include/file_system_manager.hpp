#pragma once
#include "AudioFileSourceFS.h"

class FileSystemManager {
  File m_settings_file;
  File m_playlist_file;

  void create_system_folder();
  void create_settings_file();
  void create_playlist_file();


  public:
  FileSystemManager();
  bool correct_sdcard_structure();
  void load_saved_settings();
  void load_saved_playlist();
  void write_current_song();
  void write_playlist();
  void write_settings();
};

