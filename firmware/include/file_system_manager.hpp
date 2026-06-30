#pragma once
#include "AudioFileSourceFS.h"

class FileSystemManager {
  File m_settings_file;
  File m_playlist_file;

  static void create_system_folder();
  static void create_settings_file();
  static void create_playlist_file();
  static void create_song_file();

  static File clear_file(const char* path);


  public:
  FileSystemManager();
  static bool correct_sdcard_structure();
  static float load_saved_settings(); // returns volume level for now
  static char* load_saved_playlist();
  static char* load_saved_song();
  static void write_current_song(const char* current_song);
  static void write_playlist(const char* playlist_root);
  static void write_settings(float volume);
};

