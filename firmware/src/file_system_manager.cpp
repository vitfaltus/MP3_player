#include "../include/file_system_manager.hpp"

#include "pin_config.hpp"

#include <SD.h>
#include <SPI.h>
void FileSystemManager::create_system_folder() {
  SD.mkdir("/system");
  create_settings_file();
  create_playlist_file();
}
void FileSystemManager::create_settings_file() {}
void FileSystemManager::create_playlist_file() {}


FileSystemManager::FileSystemManager() {
    SPI.begin(PinConfig::SD_SCK, PinConfig::SD_MISO, PinConfig::SD_MOSI);

    while (true) {
      if (SD.begin(PinConfig::SD_CS)) {
        return;
      }
      delay(1000);
    }
}
bool FileSystemManager::correct_sdcard_structure() {
  if (!SD.exists("/songs") || !SD.exists("/songs/all")) {return false;}
  if (!SD.exists("/system")) {
    create_system_folder();
  }
  else if (!SD.exists("/system/settings.json")) {
    create_settings_file();
  }
  else if (!SD.exists("/system/playlist.json")) {
    create_playlist_file();
  }
  return true;
}

void FileSystemManager::load_saved_settings() {}
void FileSystemManager::load_saved_playlist() {}
void FileSystemManager::write_current_song() {}
void FileSystemManager::write_playlist() {}
void FileSystemManager::write_settings() {}