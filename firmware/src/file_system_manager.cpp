#include "file_system_manager.hpp"

#include "pin_config.hpp"

#include <SD.h>
#include <SPI.h>

#define CURRENT_SONG_FILE "/.current_song_path"


bool FileSystemManager::filesys_setup() {
  SPI.begin(PinConfig::SD_SCK, PinConfig::SD_MISO, PinConfig::SD_MOSI);
  for (int i = 0; i < 5; i++) {
    if (!SD.begin(PinConfig::SD_CS)) {
      delay(1000);
    } else {
      return true; // SD card mounted
    }
  }
  return false; // SD card not mounted
}

FileSystemManager::FileSystemManager() {
  if (!filesys_setup()) {
   return;
  }
  file_system_present = true;

}

bool FileSystemManager::get_current_song_path(std::vector<char> &path_buffer) const {
  if (!file_system_present && !filesys_setup()) {
    return false;
  }
  if (!path_buffer.empty()) {path_buffer.clear();}

  File current_song_path = SD.open(CURRENT_SONG_FILE, FILE_READ);

  while (current_song_path.available()) {
    path_buffer.push_back( static_cast<char>(current_song_path.read()));
  }
  current_song_path.close();

  return true;



}
bool FileSystemManager::set_current_song_path(const std::vector<char> &path_buffer) const {
  if (!file_system_present && !filesys_setup()) {
    return false;
  }

  SD.remove(CURRENT_SONG_FILE);

  File current_song_path = SD.open(CURRENT_SONG_FILE, FILE_WRITE);

  for ( auto it: path_buffer) {
    current_song_path.write(it);
  }

  current_song_path.close();
  return true;
}
