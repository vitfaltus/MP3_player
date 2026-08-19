#pragma once
#define CURRENT_SONG_FILE "/.current_song_path"
#define DEFAULT_VOLUME_FILE "/.default_volume"
#define TIMEOUT_TIME_FILE "/.timeout_time"
#include <cstdint>


class FileSystemManager {
  bool FileSystemPresent = false;

  static bool setup();

  public:
  FileSystemManager();

  bool getCurrentSongPath(char** path) const;
  bool setCurrentSongPath(const char* path) const;


    [[nodiscard]] float getDefaultVolume() const;
    bool setDefaultVolume(float volume) const;

    [[nodiscard]] uint8_t getTimeoutTimeSeconds() const;
    bool setTimeoutTime(uint8_t time_seconds) const;




};

