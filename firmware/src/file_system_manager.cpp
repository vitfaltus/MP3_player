#include "../include/file_system_manager.hpp"

#include "pin_config.hpp"

#include <SD.h>
#include <SPI.h>

#define VOLUME_STRING  "Volume: "
#define SETTINGS_PATH  "/system/settings.txt"

#define PLAYLIST_ROOT_STRING "Playlist root: "
#define PLAYLIST_PATH "/system/playlist.txt"

#define CURRENT_PLAYING_STRING "Current song: "
#define SONG_PATH "/system/song.txt"


void FileSystemManager::create_system_folder() {
  SD.mkdir("/system");
  create_settings_file();
  create_playlist_file();
}
void FileSystemManager::create_settings_file() {
  File f = SD.open(SETTINGS_PATH, FILE_WRITE);
  // volume settings
  f.print(VOLUME_STRING);
  f.println(0.3);

  f.close();
}
void FileSystemManager::create_playlist_file() {
  File f = SD.open(PLAYLIST_PATH, FILE_WRITE);
  f.print(PLAYLIST_ROOT_STRING);
  f.println("/songs/all");
  f.println(CURRENT_PLAYING_STRING);

  f.close();
}
void FileSystemManager::create_song_file() {
  File f = SD.open(SONG_PATH, FILE_WRITE);
  f.println(CURRENT_PLAYING_STRING);

  f.close();
}
File FileSystemManager::clear_file(const char *path) {
  if (SD.exists(path)) {SD.remove(path);}

  return SD.open(path, SETTINGS_PATH);
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
  else if (!SD.exists(SETTINGS_PATH)) {
    create_settings_file();
  }
  else if (!SD.exists(PLAYLIST_PATH)) {
    create_playlist_file();
  }
  else if (!SD.exists(SONG_PATH)) {
    create_song_file();
  }
  return true;
}

float FileSystemManager::load_saved_settings() {
  File f = SD.open(PLAYLIST_PATH, FILE_WRITE);
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

char* FileSystemManager::load_saved_playlist() {
  File f = SD.open(PLAYLIST_PATH, FILE_WRITE);
  // read the header
  for (int i = 0; i < strlen(PLAYLIST_ROOT_STRING); i++) {
    f.read();
  }

  int buff_size1 = 127;
  auto playlist_root = new char[buff_size1];
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

  f.close();

  return playlist_root;

}

char *FileSystemManager::load_saved_song() {
  File f = SD.open(SONG_PATH, FILE_WRITE);
  // read the header
  for (int i = 0; i < strlen(CURRENT_PLAYING_STRING); i++) {
    f.read();
  }

  int buff_size2 = 127;
  auto current_song = new char[buff_size2];
  int cnt2 = 1;
  int read2 = f.read();
  if (read2 == '\n') {
    f.close();
    delete[] current_song;
    return nullptr;
  }
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

  return current_song;
}

void FileSystemManager::write_current_song(const char *current_song) {
  File f = clear_file(SONG_PATH);

  f.println(current_song);

  f.close();

}
void FileSystemManager::write_playlist(const char *playlist_root) {
  File f = clear_file(PLAYLIST_PATH);

  f.println(playlist_root);

  f.close();
}
void FileSystemManager::write_settings(float volume) {
  File f = clear_file(SETTINGS_PATH);

  f.println(volume);

  f.close();
}
