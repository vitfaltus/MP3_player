#include "../include/display_handler.hpp"
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <string>

#include "fonts/Org_01.h"

DisplayHandler::DisplayHandler()
{
    this->display = Adafruit_SSD1306(Width, Height, &Wire, -1);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
    }
    delay(15);
    display.setFont(&Org_01);
    drawOnBoot();
}
DisplayHandler::~DisplayHandler()
{
        delete SongName;
}

void DisplayHandler::initTopBanner(const float volt, const float vol, const bool sd)
{
    topBanner.voltage = volt;
    topBanner.volume = static_cast<uint8_t>(100*vol);
    topBanner.sdPresent = sd;

    drawTopBanner();
}

void DisplayHandler::drawTopBanner()
{
    changeBatteryVoltage(topBanner.voltage);
    changeVolumeLevel(0.01f * topBanner.volume);

    display.setCursor(117, 11);
    display.print("V");

    display.setCursor(2, 11);
    display.print("VOL:");
    display.drawBitmap(39, 6, image_Volup_bits, 8, 6, 1);

    if (topBanner.sdPresent)
        display.drawBitmap(56, 1, image_micro_sd_bits, 16, 14, 1);

    display.drawLine(0, 15, 127, 15, 1);

    display.display();
}

void DisplayHandler::changeSongName(const char* song_path)
{
    SongName = parseName(song_path);

    display.setTextWrap(false);
    display.fillRect(0, 19, 122, 19, 0x0000);
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setCursor(15, 28);
    display.print(SongName);
    display.display();
}
void DisplayHandler::changeBatteryVoltage(const float battery_voltage)
{
    topBanner.voltage = battery_voltage;

    display.setTextSize(1);

    display.fillRect(93, 6, 20, 7, 0x0000);
    display.setCursor(94, 11);
    display.println(battery_voltage);
    display.display();
}
void DisplayHandler::changeVolumeLevel(const float volume_level)
{
    topBanner.volume = static_cast<uint8_t>(100*volume_level);

    display.setTextSize(1);

    display.fillRect(24, 7, 15, 5, 0x0000);
    display.setCursor(24, 11);
    display.print(topBanner.volume);
    display.display();
}
void DisplayHandler::drawPause()
{
    display.fillEllipse(64, 52, 7, 7, 0);
    display.drawBitmap(57, 45, image_music_pause_button_bits, 15, 16, 1);
    display.display();
}
void DisplayHandler::drawPlay()
{
    display.fillEllipse(64, 52, 7, 7, 0);
    display.drawBitmap(57, 45, image_music_play_button_bits, 15, 16, 1);
    display.display();
}
void DisplayHandler::dimScreen(const bool cond) { display.dim(cond); }

void DisplayHandler::drawSongTemplate()
{
    display.clearDisplay();

    drawTopBanner();

    // passport_left
    display.drawBitmap(122, 18, image_passport_left_bits, 6, 46, 1);
    // earth
    display.drawBitmap(1, 47, image_earth_bits, 15, 16, 1);
    // paint 15
    display.drawBitmap(94, 44, image_paint_15_bits, 28, 27, 1);
    // paint 15
    display.drawBitmap(-9, 36, image_paint_15_1_bits, 48, 27, 1);
    // network
    display.drawBitmap(41, 49, image_network_bits, 15, 16, 1);
    // network
    display.drawBitmap(73, 49, image_network_1_bits, 15, 16, 1);
    // rect 20
    display.drawRect(57, 60, 3, 4, 1);
    // rect 20
    display.drawRect(69, 60, 3, 4, 1);
    // rect 22
    display.drawRect(61, 61, 3, 3, 1);
    // rect 22
    display.drawRect(65, 61, 3, 3, 1);

    display.display();
}
void DisplayHandler::showSongScreen(const char* song_path,
                                      const float battery_voltage, const float volume_level)
{

    // Display.setTextSize(1);
    // Display.setCursor(35, 40);
    // Display.print("Artist Name");
    // string 4
    //Display.setCursor(94, 11);

    dimScreen(true);
    drawSongTemplate();
    changeVolumeLevel(volume_level);
    changeSongName(song_path);
    changeBatteryVoltage(battery_voltage);
    drawPause();
    dimScreen(false);
}
void DisplayHandler::drawMenuScreen(const uint8_t selector_positon)
{
    display.clearDisplay();
    display.setTextSize(1);

    display.setTextWrap(false);

    display.setCursor(4, 18);
    display.println("Songs selection");

    display.setCursor(4, 33);
    display.println("Settings");

    display.setCursor(4, 48);
    display.println("Debug info");

    display.drawRoundRect(2, 16 + selector_positon*15, 95, 12, 3, SSD1306_WHITE);

    display.display();
}

void DisplayHandler::drawDebugScreen(multi_heap_info_t& info)
{

    uint8_t tmp_display_shift = 15;
    heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);

    display.clearDisplay();

    display.setTextWrap(false);
    display.setCursor(4, 18+tmp_display_shift);
    display.println("Free: ");

    display.setCursor(4, 33+tmp_display_shift);
    display.println("Allocated: ");

    display.setCursor(68, 18+tmp_display_shift);
    display.println(info.total_free_bytes);

    display.setCursor(68, 33+tmp_display_shift);
    display.println(info.total_allocated_bytes);

    display.setCursor(115, 18+tmp_display_shift);
    display.println("B");

    display.setCursor(115, 33+tmp_display_shift);
    display.println("B");


    display.display();
}

void DisplayHandler::drawSongSelectScreen(const std::array<char*, 3>& DisplaySongArr, uint8_t selector_positon)
{
    display.clearDisplay();
    display.setTextSize(1);

    display.setTextWrap(false);

    display.setCursor(4, 18);
    display.println(DisplaySongArr[0]);

    display.setCursor(4, 33);
    display.println(DisplaySongArr[1]);

    display.setCursor(4, 48);
    display.println(DisplaySongArr[2]);

    display.drawRoundRect(2, 16 + selector_positon*15, 126, 12, 3, SSD1306_WHITE);

    display.display();
}

void DisplayHandler::drawSettingsScreen(uint8_t selector_positon)
{
    display.clearDisplay();
    display.setTextSize(1);

    display.setTextWrap(false);

    display.setCursor(4, 18);
    display.println("Default volume");

    display.setCursor(4, 33);
    display.println("Timeout time");

    display.drawRoundRect(2, selector_positon*15, 126, 12, 3, SSD1306_WHITE);

    display.display();
}

void DisplayHandler::drawSettingsDefaultVolume(float volume_level)
{
    display.fillScreen(0x0);

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setCursor(42, 22);
    display.println(volume_level);

    display.fillRect(10, 50, 18, 4, SSD1306_WHITE);

    display.fillRect(100, 50, 18, 4, SSD1306_WHITE);

    display.fillRect(107, 43, 4, 18, SSD1306_WHITE);

    display.display();
}

void DisplayHandler::drawSettingsTimeOut(int timeout)
{
    display.fillScreen(0x0);

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(3);
    display.setTextWrap(false);
    display.setCursor(39, 21);
    display.println(timeout);

    display.setCursor(79, 20);
    display.println("s");

    display.fillRect(10, 50, 18, 4, SSD1306_WHITE);

    display.fillRect(100, 50, 18, 4, SSD1306_WHITE);

    display.fillRect(107, 43, 4, 18, SSD1306_WHITE);

    display.display();
}

int DisplayHandler::getScreenTimeoutSeconds() const
{
    return DimmingTimerMs/1000;
}
void DisplayHandler::setScreenTimeoutSeconds(const int seconds)
{
    DimmingTimerMs = seconds*1000;
}

void DisplayHandler::incrementTimeout()
{
    if (DimmingTimerMs < MAX_TIMEOUT_SECONDS*1000)
    {
        DimmingTimerMs += 1000;
    }
}

void DisplayHandler::decrementTimeout()
{
    if (DimmingTimerMs > MIN_TIMEOUT_SECONDS*1000)
    {
        DimmingTimerMs -= 1000;
    }
}

void DisplayHandler::displayErrorMessage(const char* error_message, const uint8_t delay_time)
{
    display.clearDisplay();

    display.setTextWrap(true);
    display.setCursor(4, 33);

    display.println(error_message);
    display.display();

    delay(delay_time*1000);
}

void DisplayHandler::drawOnBoot()
{
    display.clearDisplay();

    display.setTextSize(3);
    display.setCursor(16, 40);
    display.setTextColor(WHITE);
    display.println("\\(^_^)/");
    display.display();
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


bool DisplayHandler::displayDimmingRoutine(
    const InputHandler::ButtonPress ButtonInput)
{
    const unsigned long now_millis = millis();

    if (ButtonInput != InputHandler::None)
    {
        LastMillis = now_millis;
        if ( ScreenDimmed)
        {
            dimScreen(false);
            ScreenDimmed = false;
            return true;
        }

    }
    if (
        now_millis - LastMillis > DimmingTimerMs)
    {
        dimScreen(true);
        ScreenDimmed = true;
        LastMillis = now_millis;
    }
    return false;


}