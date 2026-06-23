#include <Arduino.h>
#include "audio_settings.hpp"
#include "battery_manager.hpp"
#include "display_handler.hpp"
#include "input_handler.hpp"
#include "pin_config.hpp"
#include "playlist.hpp"

InputHandler* input_handler;
AudioSettings* audio_settings;
Playlist* playlist;
DisplayHandler* display_handler;
BatteryManager* battery_manager;

bool filesys_setup()
{
    SPI.begin(PinConfig::SD_SCK, PinConfig::SD_MISO, PinConfig::SD_MOSI);
    for (int i = 0; i < 5; i++)
    {
        if (!SD.begin(PinConfig::SD_CS))
        {
            delay(1000);
        }
        else
        {
            return true; // SD card mounted
        }
    }
    return false; // SD card not mounted
}

void setup()
{
    Serial.begin(9600);

    Serial.println("Display setup");
    display_handler = new DisplayHandler();
    Serial.println("Display set up");

    battery_manager = new BatteryManager();

    Serial.println("Setting up filesystem");
    if (filesys_setup())
    {
        Serial.println("Filesystem set up");
    }
    else
    {
        Serial.println("Filesystem failed to set up");
    }

    Serial.println("Creating components");

    input_handler = new InputHandler();
    audio_settings = new AudioSettings();

    Serial.println("Creating playlist");
    playlist = new Playlist("/");

    Serial.println("Systems set up");
    display_handler->showSongScreen(playlist->getSongName(),
                                      BatteryManager::getBatteryVoltage(),
                                      audio_settings->getVolume());

    Serial.println("Buttons calibrated");
}

unsigned long last_millis = 0;
int read_interval_millis = 5000;
void loop()
{

    InputHandler::ButtonPress button_input = input_handler->checkButtons();

    if (button_input != InputHandler::ButtonPress::None)
    {

        Serial.println(button_input);
    }
    switch (button_input)
    {
    case InputHandler::LeftButtonPress: // volume down
        audio_settings->volumeDown();
        display_handler->changeVolumeLevel(audio_settings->getVolume());
        break;
    case InputHandler::LeftButtonLongPress: // previous song
        playlist->playPreviousSong(audio_settings);
        display_handler->changeSongName(playlist->getSongName());
        break;
    case InputHandler::MiddleButtonPress: // pause/play song
        if (playlist->isPaused())
        {
            playlist->play(audio_settings);
            audio_settings->restoreAudio();
            display_handler->drawPlay();
        }
        else
        {
            playlist->stop();
            audio_settings->shutAudio();
            display_handler->drawPause();
        }
        break;
    case InputHandler::RightButtonPress: // volume up
        audio_settings->volumeUp();
        display_handler->changeVolumeLevel(audio_settings->getVolume());
        break;
    case InputHandler::RightButtonLongPress: // next song
        playlist->playNextSong(audio_settings);
        display_handler->changeSongName(playlist->getSongName());
        break;
    default:
        break;
    }
    playlist->playlistLoop(audio_settings);
    if (unsigned long now_millis = millis();
        now_millis - last_millis > read_interval_millis)
    {
        display_handler->changeBatteryVoltage(
            BatteryManager::getBatteryVoltage());
        last_millis = now_millis;
    }
}
