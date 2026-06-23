#ifndef FIRMWARE_INCLUDE_DISPLAY_HANDLER_HPP
#define FIRMWARE_INCLUDE_DISPLAY_HANDLER_HPP


#include <Adafruit_SSD1306.h>

class DisplayHandler
{
    Adafruit_SSD1306 Display;
    int Width = 128, Height = 64;
    float BatteryVoltage = 0;
    float VolumeLevel = 0;
    char* SongName = nullptr;
    void drawOnBoot();
    static char* parseName(const char* song_path);

public:
    DisplayHandler();
    ~DisplayHandler();
    void changeSongName(const char* song_path);
    void changeBatteryVoltage(float battery_voltage);
    void changeVolumeLevel(float volume_level);
    void drawPause();
    void drawPlay();
    void dimScreen(bool cond);
    void drawSongTemplate();
    void showSongScreen(const char* song_path, float battery_voltage,
                          float volume_level);
};

#endif
