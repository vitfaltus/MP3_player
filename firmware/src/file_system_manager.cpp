#include "file_system_manager.hpp"

#include "pin_config.hpp"

#include <SD.h>
#include <SPI.h>



bool C_FileSystemManager::setup() {
  SPI.begin(PinConfig::SD_SCK, PinConfig::SD_MISO, PinConfig::SD_MOSI);
  for (int i = 0; i < 5; i++) {
    if (!SD.begin(PinConfig::SD_CS, SPI, 4000000)) {
        Serial.println("SDC mounting failed");
        delay(1000);
    } else {
      return true; // SD card mounted
    }
  }
  return false; // SD card not mounted
}

C_FileSystemManager::C_FileSystemManager() {
  if (!setup()) {
   return;
  }
  FileSystemPresent = true;

}


// when returns true -> memory has been allocated
bool C_FileSystemManager::getCurrentSongPath(char** path) const {
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
    *path = new char[1 + file_size];
    (*path)[file_size] = '\0';

    size_t counter = 0;
  while (SongFile.available() && counter < file_size) {
    (*path)[counter] = SongFile.read();
      Serial.print((*path)[counter]);
      counter++;
  }
  SongFile.close();

  return true;

}
void C_FileSystemManager::setCurrentSongPath(const char* path) const {
  if (!FileSystemPresent && !setup()) {
    return;
  }

  SD.remove(CURRENT_SONG_FILE);

  File SongFile = SD.open(CURRENT_SONG_FILE, FILE_WRITE);

  const char* counter = path;
    while (*counter != '\0')
    {
        SongFile.write(*counter);
        counter++;
    }

  SongFile.close();
}

float C_FileSystemManager::getDefaultVolume() const
{
    if (!FileSystemPresent && !setup() || !SD.exists(DEFAULT_VOLUME_FILE)) {
        return 0.3f;
    }

    File f = SD.open(DEFAULT_VOLUME_FILE, FILE_READ);

    constexpr uint8_t BufferSize = 5;

    char buffer[BufferSize];

    for (uint8_t i = 0; i < BufferSize-1; i++)
    {
        if (!f.available())
        {
            break;
        }
        buffer[i] = f.read();
    }
    buffer[BufferSize-1] = '\0';
    const float result = atof(buffer);

    f.close();
    return result;
}

void C_FileSystemManager::setDefaultVolume(float volume) const
{

    if (!FileSystemPresent && !setup()) {
        return;
    }

    SD.remove(DEFAULT_VOLUME_FILE);

    File f = SD.open(DEFAULT_VOLUME_FILE, FILE_WRITE);

    constexpr uint8_t BufferSize = 5;
    char buffer[BufferSize];
    // truncation doesnt matter since we want only two points of precision
    if (snprintf(buffer, BufferSize, "%f", volume) < 0)
    {
        return;
    }

    for (const char c : buffer)
    {
        f.write(c);
    }

    f.close();

}

uint8_t C_FileSystemManager::getTimeoutTimeSeconds() const
{
    if (!FileSystemPresent && !setup() || !SD.exists(TIMEOUT_TIME_FILE)) {
        return 20;
    }

    File f = SD.open(TIMEOUT_TIME_FILE, FILE_READ);

    const uint8_t result = f.read();

    f.close();
    return result;
}

void C_FileSystemManager::setTimeoutTime(const uint8_t time_seconds) const
{
    if (!FileSystemPresent && !setup()) {
        return;
    }

    SD.remove(TIMEOUT_TIME_FILE);

    File f = SD.open(TIMEOUT_TIME_FILE, FILE_WRITE);

    f.write(time_seconds);

    f.close();

}
