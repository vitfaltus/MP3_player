#include "audio_settings.hpp"
#include "battery_manager.hpp"
#include "display_handler.hpp"
#include "input_handler.hpp"
#include "pin_config.hpp"
#include "playlist.hpp"
#include <Arduino.h>

InputHandler *input_handler;
AudioSettings *audio_settings;
Playlist *playlist;
DisplayHandler *display_handler;
BatteryManager *battery_manager;

bool filesys_setup() {
  SPI.begin(PinConfig::SD_SCK, PinConfig::SD_MISO, PinConfig::SD_MOSI);
  for (int i = 0; i < 5; i++) {
    if (!SD.begin(PinConfig::SD_CS)) {
      delay(1000);
    } else {
      return true; // SD card mounted
    }
  }
  return false; // SD card not mounted
}

void setup() {
  Serial.begin(9600);

  Serial.println("Display setup");
  display_handler = new DisplayHandler();
  Serial.println("Display set up");

  battery_manager = new BatteryManager();

  Serial.println("Setting up filesystem");
  if (filesys_setup()) {
    Serial.println("Filesystem set up");
  } else {
    Serial.println("Filesystem failed to set up");
  }

  Serial.println("Creating components");

  input_handler = new InputHandler();
  audio_settings = new AudioSettings();

  Serial.println("Creating playlist");
  playlist = new Playlist("/");

  Serial.println("Systems set up");
  display_handler->show_song_screen(playlist->GetSongName(),
                                    BatteryManager::get_battery_voltage(),
                                    audio_settings->get_volume());

  input_handler->buttons_calibration();
  Serial.println("Buttons calibrated");
}

unsigned long last_millis = 0;
int read_interval_millis = 5000;
void loop() {

  InputHandler::button_press button_input = input_handler->button_check();

  if (button_input != InputHandler::button_press::none) {

    Serial.println(button_input);
  }
  switch (button_input) {
  case InputHandler::left_button_press: // volume down
    audio_settings->volume_down();
    display_handler->change_volume_level(audio_settings->get_volume());
    break;
  case InputHandler::left_button_long_press: // previous song
    playlist->PlayPreviousSong(audio_settings);
    display_handler->change_song_name(playlist->GetSongName());
    break;
  case InputHandler::middle_button_press: // pause/play song
    if (playlist->IsPaused()) {
      playlist->Play(audio_settings);
      audio_settings->restore_audio();
      display_handler->draw_play();
    } else {
      playlist->Stop();
      audio_settings->shut_audio();
      display_handler->draw_pause();
    }
    break;
  case InputHandler::right_button_press: // volume up
    audio_settings->volume_up();
    display_handler->change_volume_level(audio_settings->get_volume());
    break;
  case InputHandler::right_button_long_press: // next song
    playlist->PlayNextSong(audio_settings);
    display_handler->change_song_name(playlist->GetSongName());
    break;
  default:
    break;
  }
  playlist->PlaylistLoop(audio_settings);
  if (unsigned long now_millis = millis();
      now_millis - last_millis > read_interval_millis) {
    display_handler->change_battery_voltage(
        BatteryManager::get_battery_voltage());
    last_millis = now_millis;
  }
}
