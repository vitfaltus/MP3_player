#ifndef FIRMWARE_INCLUDE_DISPLAY_HANDLER_HPP
#define FIRMWARE_INCLUDE_DISPLAY_HANDLER_HPP


#include <Adafruit_SSD1306.h>

#include "input_handler.hpp"

class DisplayHandler
{
    Adafruit_SSD1306 Display;

    int Width = 128, Height = 64;
    float BatteryVoltage = 0;
    float VolumeLevel = 0;
    char* SongName = nullptr;


    unsigned long LastMillis= 0;
    int ReadIntervalMillis = 20000;
    bool ScreenDimmed = false;

    void drawOnBoot();
    static char* parseName(const char* song_path);
    void dimScreen(bool cond);

public:
    DisplayHandler();
    ~DisplayHandler();

    void changeSongName(const char* song_path);
    void changeBatteryVoltage(float battery_voltage);
    void changeVolumeLevel(float volume_level);
    void drawPause();
    void drawPlay();
    // This method returns true, if the display was turned on by the input.
    bool displayDimmingRoutine(InputHandler::ButtonPress ButtonInput);
    // Draws const labels on screen and resets the font size.
    void drawSongTemplate();
    void showSongScreen(const char* song_path, float battery_voltage,
                          float volume_level);


    int getScreenTimeoutSeconds() const;
    void setScreenTimeoutSeconds(int seconds);
};

#endif
