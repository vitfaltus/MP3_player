#include "../include/file_system_manager.hpp"

#include "pin_config.hpp"

#include <SD.h>
#include <SPI.h>

#define VOLUME_STRING  "Volume: "
#define PLAYLIST_ROOT_STRING "Playlist root: "
#define CURRENT_PLAYING_STRING "Current song: "


void FileSystemManager::create_system_folder() {
  SD.mkdir("/system");
  create_settings_file();
  create_playlist_file();
}
void FileSystemManager::create_settings_file() {
  File f = SD.open("/system/settings.txt", FILE_WRITE);
  // volume settings
  f.print(VOLUME_STRING);
  f.println(0.3);

  f.close();
}
void FileSystemManager::create_playlist_file() {
  File f = SD.open("/system/playlist.txt", FILE_WRITE);
  f.print(PLAYLIST_ROOT_STRING);
  f.println("/songs/all");
  f.println(CURRENT_PLAYING_STRING);

  f.close();
}


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
  else if (!SD.exists("/system/settings.txt")) {
    create_settings_file();
  }
  else if (!SD.exists("/system/playlist.txt")) {
    create_playlist_file();
  }
  return true;
}

float FileSystemManager::load_saved_settings() {
  File f = SD.open("/system/settings.txt", FILE_WRITE);
  // read the header
  for (int i = 0; i < strlen(VOLUME_STRING); i++) {
    f.read();
  }

  constexpr int buff_size = 4;
  u_int8_t buff[buff_size];
  f.read(buff, buff_size);
  f.close();

  // convert u_int8_t array to float
  return strtof(reinterpret_cast<const char *>(buff), nullptr);
}

void FileSystemManager::load_saved_playlist(char *playlist_root,
char *current_song) {
  File f = SD.open("/system/playlist.txt", FILE_WRITE);
  // read the header first line
  for (int i = 0; i < strlen(PLAYLIST_ROOT_STRING); i++) {
    f.read();
  }

  //first line
  int buff_size1 = 127;
  playlist_root = new char[buff_size1];
  int cnt1 = 1;
  int read1 = f.read();
  playlist_root[0] = read1;
  while (read1 != static_cast<int>('\n')) {
    if (cnt1 < buff_size1 - 1) {
      buff_size1 *= 2;
      auto tmp_buff = new char[buff_size1];
      memcpy(tmp_buff, playlist_root, buff_size1/2);
      delete[] playlist_root;
      playlist_root = tmp_buff;
    }
    read1 = f.read();
    playlist_root[cnt1] = read1;
    cnt1++;
  }
  playlist_root[cnt1] = '\n';


  // TODO check for null -> current_song = nullptr
  // read the header second line
  for (int i = 0; i < strlen(CURRENT_PLAYING_STRING); i++) {
    f.read();
  }

  //second line
  int buff_size2 = 127;
  current_song = new char[buff_size2];
  int cnt2 = 1;
  int read2 = f.read();
  current_song[0] = read2;
  while (read2 != static_cast<int>('\n')) {
    if (cnt2 < buff_size2 - 1) {
      buff_size2 *= 2;
      auto tmp_buff2 = new char[buff_size2];
      memcpy(tmp_buff2, current_song, buff_size2/2);
      delete[] current_song;
      current_song = tmp_buff2;
    }
    read2 = f.read();
    current_song[cnt2] = read2;
    cnt2++;
  }
  current_song[cnt2] = '\n';

  f.close();
}

void FileSystemManager::write_current_song(const char *current_song) {

}
void FileSystemManager::write_playlist(const char *playlist_root,
                                       const char *current_song) {

}
void FileSystemManager::write_settings(float volume) {

}
