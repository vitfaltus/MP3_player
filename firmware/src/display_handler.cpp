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
        delete SongName;
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
void DisplayHandler::changeBatteryVoltage(const float battery_voltage)
{
    BatteryVoltage = battery_voltage;

    Display.setTextWrap(false);
    Display.setTextSize(1);

    Display.fillRect(109, 0, 128, 12, 0x0000);
    Display.setCursor(111, 3);
    Display.println(battery_voltage);
    Display.display();
}
void DisplayHandler::changeVolumeLevel(const float volume_level)
{
    VolumeLevel = volume_level;

    Display.setTextWrap(false);

    Display.fillRect(24, 1, 30, 12, 0x0000);
    Display.setCursor(25, 3);
    Display.println(VolumeLevel);
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
                                      const float battery_voltage, const float volume_level)
{
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
    Display.clearDisplay();
    Display.setTextSize(1);

    Display.setTextWrap(false);

    Display.setCursor(4, 18);
    Display.println("Songs selection");

    Display.setCursor(4, 33);
    Display.println("Settings");

    Display.setCursor(4, 48);
    Display.println("Debug info");

    Display.drawRoundRect(2, 16 + selector_positon*15, 95, 12, 3, SSD1306_WHITE);

    Display.display();
}

void DisplayHandler::drawDebugScreen(multi_heap_info_t& info)
{

    uint8_t tmp_display_shift = 15;
    heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);

    Display.clearDisplay();

    Display.setTextWrap(false);
    Display.setCursor(4, 18+tmp_display_shift);
    Display.println("Free: ");

    Display.setCursor(4, 33+tmp_display_shift);
    Display.println("Allocated: ");

    Display.setCursor(68, 18+tmp_display_shift);
    Display.println(info.total_free_bytes);

    Display.setCursor(68, 33+tmp_display_shift);
    Display.println(info.total_allocated_bytes);

    Display.setCursor(115, 18+tmp_display_shift);
    Display.println("B");

    Display.setCursor(115, 33+tmp_display_shift);
    Display.println("B");


    Display.display();
}

void DisplayHandler::drawSongSelectScreen(const std::array<char*, 3>& DisplaySongArr, uint8_t selector_positon)
{
    Display.clearDisplay();
    Display.setTextSize(1);

    Display.setTextWrap(false);

    Display.setCursor(4, 18);
    Display.println(DisplaySongArr[0]);

    Display.setCursor(4, 33);
    Display.println(DisplaySongArr[1]);

    Display.setCursor(4, 48);
    Display.println(DisplaySongArr[2]);

    Display.drawRoundRect(2, 16 + selector_positon*15, 126, 12, 3, SSD1306_WHITE);

    Display.display();
}

void DisplayHandler::drawSettingsScreen(uint8_t selector_positon)
{
    Display.clearDisplay();
    Display.setTextSize(1);

    Display.setTextWrap(false);

    Display.setCursor(4, 18);
    Display.println("Default volume");

    Display.setCursor(4, 33);
    Display.println("Timeout time");

    Display.drawRoundRect(2, 16 + selector_positon*15, 126, 12, 3, SSD1306_WHITE);

    Display.display();
}

void DisplayHandler::drawSettingsDefaultVolume(float volume_level)
{
    Display.fillScreen(0x0);

    Display.setTextColor(SSD1306_WHITE);
    Display.setTextSize(2);
    Display.setTextWrap(false);
    Display.setCursor(42, 22);
    Display.println(volume_level);

    Display.fillRect(10, 50, 18, 4, SSD1306_WHITE);

    Display.fillRect(100, 50, 18, 4, SSD1306_WHITE);

    Display.fillRect(107, 43, 4, 18, SSD1306_WHITE);

    Display.display();
}

void DisplayHandler::drawSettingsTimeOut(int timeout)
{
    Display.fillScreen(0x0);

    Display.setTextColor(SSD1306_WHITE);
    Display.setTextSize(3);
    Display.setTextWrap(false);
    Display.setCursor(39, 21);
    Display.println(timeout);

    Display.setCursor(79, 20);
    Display.println("s");

    Display.fillRect(10, 50, 18, 4, SSD1306_WHITE);

    Display.fillRect(100, 50, 18, 4, SSD1306_WHITE);

    Display.fillRect(107, 43, 4, 18, SSD1306_WHITE);

    Display.display();
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
    Display.clearDisplay();

    Display.setTextWrap(true);
    Display.setCursor(4, 33);

    Display.println(error_message);
    Display.display();

    delay(delay_time*1000);
}

void DisplayHandler::drawOnBoot()
{
    Display.clearDisplay();

    Display.setTextSize(3);
    Display.setCursor(2, 30);
    Display.setTextColor(WHITE);
    Display.println("\\(^_^)/");
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