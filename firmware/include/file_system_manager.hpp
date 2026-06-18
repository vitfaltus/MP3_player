#pragma once
#include "AudioFileSourceFS.h"

class FileSystemManager {
  File m_settings_file;
  File m_playlist_file;

  static void create_system_folder();
  static void create_settings_file();
  static void create_playlist_file();


  public:
  FileSystemManager();
  static bool correct_sdcard_structure();
  static float load_saved_settings(); // returns volume level for now
  static void load_saved_playlist(char* playlist_root, char* current_song);
  void write_current_song(const char* current_song);
  void write_playlist(const char* playlist_root, const char* current_song);
  void write_settings(float volume);
};

