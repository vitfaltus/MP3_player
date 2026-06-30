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


// when returns true -> memory has been allocated
bool FileSystemManager::getCurrentSongPath(char* path) const {
  if (!FileSystemPresent && !setup()) {
    return false;
  }
    if (!SD.exists(CURRENT_SONG_FILE))
    {
        return false;
    }

  File SongFile = SD.open(CURRENT_SONG_FILE, FILE_READ);

    const size_t file_size = SongFile.size();
    if (file_size == 0)
    {
        return false;
    }
    path = new char[1 + file_size];
    path[file_size] = '\0';

    size_t counter = 0;
  while (SongFile.available()) {
    path[counter] = SongFile.read();
      counter++;
  }
  SongFile.close();

  return true;

}
bool FileSystemManager::setCurrentSongPath(const char* path) const {
  if (!FileSystemPresent && !setup()) {
    return false;
  }

  SD.remove(CURRENT_SONG_FILE);

  File SongFile = SD.open(CURRENT_SONG_FILE, FILE_WRITE);
    Serial.print("Writing song: ");
    Serial.println(path);
  const char* counter = path;
    while (*counter != '\0')
    {
        SongFile.write(*counter);
        counter++;
    }

  SongFile.close();
  return true;
}
