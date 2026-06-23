#include "../include/display_handler.hpp"
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <string>

DisplayHandler::DisplayHandler()
{
    this->Display = Adafruit_SSD1306(Width, Height, &Wire, -1);

    if (!Display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
    }
    delay(15);
    drawOnBoot();
    Display.setTextSize(1);
    Display.clearDisplay();
}
DisplayHandler::~DisplayHandler()
{
    if (SongName)
    {
        delete SongName;
    }
}
void DisplayHandler::changeSongName(const char* song_path)
{
    SongName = parseName(song_path);

    Display.setTextWrap(true);
    Display.fillRect(0, 26, 128, 38, 0x0000);
    Display.setCursor(19, 35);
    Display.println(SongName);
    Display.display();
}
void DisplayHandler::changeBatteryVoltage(float battery_voltage)
{
    BatteryVoltage = battery_voltage;

    Display.setTextWrap(false);

    Display.fillRect(109, 0, 128, 12, 0x0000);
    Display.setCursor(111, 3);
    Display.println(battery_voltage);
    Display.display();
}
void DisplayHandler::changeVolumeLevel(float volume_level)
{
    VolumeLevel = volume_level;

    Display.setTextWrap(false);

    Display.fillRect(24, 1, 30, 12, 0x0000);
    Display.setCursor(25, 3);
    Display.println(volume_level);
    Display.display();
}
void DisplayHandler::drawPause()
{
    Display.setTextWrap(false);

    Display.fillRect(55, 2, 30, 10, 0x0000);
    Display.setCursor(55, 2);
    Display.println("Psd");
    Display.display();
}
void DisplayHandler::drawPlay()
{
    Display.setTextWrap(false);
    Display.fillRect(55, 2, 30, 10, 0x0000);
    Display.setCursor(55, 2);
    Display.println("Ply");
    Display.display();
}
void DisplayHandler::dimScreen(const bool cond) { Display.dim(cond); }

void DisplayHandler::drawSongTemplate()
{
    Display.clearDisplay();
    Display.setTextSize(1);

    Display.setTextWrap(false);
    Display.setCursor(87, 3);
    Display.println("Bat:");

    Display.setCursor(5, 17);
    Display.println("Now playing:");

    Display.setCursor(2, 3);
    Display.println("Vol");

    Display.display();
}
void DisplayHandler::showSongScreen(const char* song_path,
                                      float battery_voltage, float volume_level)
{
    dimScreen(true);
    drawSongTemplate();
    changeVolumeLevel(volume_level);
    changeSongName(song_path);
    changeBatteryVoltage(battery_voltage);
    drawPause();
    dimScreen(false);
}
void DisplayHandler::drawOnBoot()
{
    Display.clearDisplay();

    Display.setTextSize(3);
    Display.setCursor(40, 20);
    Display.setTextColor(WHITE);
    Display.println(":3");
    Display.display();
}
char* DisplayHandler::parseName(const char* song_path)
{
    size_t last_slash_idx = 0;
    size_t current_idx = 0;
    while (song_path[current_idx] != '\0')
    {
        if (song_path[current_idx] == '/')
        {
            last_slash_idx = current_idx;
        }
        current_idx++;
    }

    if (last_slash_idx > strlen(song_path) - 1)
    {
        return nullptr;
    }

    const char* tmp_name = song_path + last_slash_idx + 1;
    char* song_name = strdup(tmp_name);
    return song_name;
}
