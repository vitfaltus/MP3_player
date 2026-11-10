#pragma once
#include <cstdint>


class C_FileSystemManager {
  bool FileSystemPresent = false;

  static bool setup();

  public:
  C_FileSystemManager();

  bool getCurrentSongPath(char** path) const;
  void setCurrentSongPath(const char* path) const;


    [[nodiscard]] float getDefaultVolume() const;
    void setDefaultVolume(float volume) const;

    [[nodiscard]] uint8_t getTimeoutTimeSeconds() const;
    void setTimeoutTime(uint8_t time_seconds) const;




};

