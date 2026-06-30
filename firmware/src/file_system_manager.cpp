#include "file_system_manager.hpp"

#include "pin_config.hpp"

#include <SD.h>
#include <SPI.h>



bool FileSystemManager::setup() {
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
  if (!setup()) {
   return;
  }
  FileSystemPresent = true;

}

bool FileSystemManager::getCurrentSongPath(std::vector<char> &path_buffer) const {
  if (!FileSystemPresent && !setup()) {
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
bool FileSystemManager::setCurrentSongPath(const std::vector<char> &path_buffer) const {
  if (!FileSystemPresent && !setup()) {
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
