#include <Arduino.h>
#include "audio_settings.hpp"
#include "battery_manager.hpp"
#include "display_handler.hpp"
#include "input_handler.hpp"
#include "pin_config.hpp"
#include "playlist.hpp"


enum DeviceState
{
    SongPlaying,
    Settings
};

InputHandler* input_handler;
AudioSettings* audio_settings;
Playlist* playlist;
DisplayHandler* display_handler;
BatteryManager* battery_manager;

DeviceState State;

unsigned long last_millis_battery = 0;
int read_interval_millis_battery = 5000;



bool mountSDCard()
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

void changeState()
{
    State = static_cast<DeviceState>((State + 1) % 2);
}


void songPlayingState(const InputHandler::ButtonPress ButtonInput)
{
    if (display_handler->displayDimmingRoutine(ButtonInput))
    {
        return;
    }

    switch (ButtonInput)
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
    case InputHandler::MiddleButtonLongPress:
        changeState();
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
    if (playlist->playlistLoop(audio_settings))
    {
        display_handler->changeSongName(playlist->getSongName());
    }
}

void settingsState(const InputHandler::ButtonPress ButtonInput)
{
    switch (ButtonInput)
    {
    case InputHandler::LeftButtonPress:

        break;
    case InputHandler::LeftButtonLongPress:

        break;
    case InputHandler::MiddleButtonPress:

        break;
    case InputHandler::MiddleButtonLongPress:
        changeState();
        break;
    case InputHandler::RightButtonPress:

        break;
    case InputHandler::RightButtonLongPress:

        break;
    default:
        break;
    }
}

void batteryRoutine()
{
    if (const unsigned long now_millis = millis();
        now_millis - last_millis_battery > read_interval_millis_battery)
    {
        display_handler->changeBatteryVoltage(
            BatteryManager::getBatteryVoltage());
        last_millis_battery = now_millis;
    }
}





void setup()
{
    Serial.begin(9600);

    display_handler = new DisplayHandler();
    Serial.println("Display setup");

    battery_manager = new BatteryManager();
    Serial.println("Battery manager setup");

    if (mountSDCard())
    {
        Serial.println("Filesystem set up");
    }
    else
    {
        Serial.println("Filesystem failed to set up");
    }

    State = SongPlaying;
    
    input_handler = new InputHandler();
    Serial.println("Input handler setup");

    audio_settings = new AudioSettings();
    Serial.println("Audio settings setup");

    playlist = new Playlist("/");
    Serial.println("Playlist created");

    display_handler->showSongScreen(playlist->getSongName(),
                                      BatteryManager::getBatteryVoltage(),
                                      audio_settings->getVolume());
    
}


void loop()
{
    const InputHandler::ButtonPress ButtonInput = input_handler->checkButtons();

    switch (State)
    {
        case SongPlaying:
            songPlayingState(ButtonInput);
        break;
        case Settings:
            settingsState(ButtonInput);
        break;
    }
    
    batteryRoutine();
}

