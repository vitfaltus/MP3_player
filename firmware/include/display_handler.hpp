#ifndef FIRMWARE_INCLUDE_DISPLAY_HANDLER_HPP
#define FIRMWARE_INCLUDE_DISPLAY_HANDLER_HPP


#include <Adafruit_SSD1306.h>

#include "input_handler.hpp"

#define MAX_TIMEOUT_SECONDS 60
#define MIN_TIMEOUT_SECONDS 5

class DisplayHandler
{
    Adafruit_SSD1306 Display;

    int Width = 128, Height = 64;
    float BatteryVoltage = 0;
    float VolumeLevel = 0;
    char* SongName = nullptr;


    unsigned long LastMillis= 0;
    int DimmingTimerMs = 20000;
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


    void drawMenuScreen(uint8_t selector_positon);

    void drawDebugScreen(multi_heap_info_t& info);

    void drawSongSelectScreen(const std::array<char*, 3>& DisplaySongArr, uint8_t selector_positon);

    void drawSettingsScreen(uint8_t selector_positon);

    void drawSettingsDefaultVolume(float volume_level);

    void drawSettingsTimeOut(int timeout);


    [[nodiscard]] int getScreenTimeoutSeconds() const;
    void setScreenTimeoutSeconds(int seconds);

    void incrementTimeout();
    void decrementTimeout();


    void displayErrorMessage(const char* error_message, uint8_t delay_time);
};

#endif
