#include <Arduino.h>
#include "pin_config.hpp"
#include "input_handler.hpp"
#include "audio_settings.hpp"
#include "playlist.hpp"

InputHandler* input_handler;
AudioSettings* audio_settings;
Playlist* playlist;

bool filesys_setup(){
  SPI.begin(PinConfig::SD_SCK, PinConfig::SD_MISO, PinConfig::SD_MOSI);
  for (int i = 0; i < 5; i++){
    if (!SD.begin(PinConfig::SD_CS)) {
      delay(1000);
    }
    else{
      return true; // SD card mounted
    }
  }
  return false; // SD card not mounted
}

void setup() {
  Serial.begin(9600);

  Serial.println("Setting up filesystem");
  filesys_setup();
  Serial.println("Filesystem set up");

  Serial.println("Creating components");

  input_handler = new InputHandler();
  audio_settings = new AudioSettings();
  Serial.println("Creating playlist");
  playlist = new Playlist("/\0");

}

void loop() {

  InputHandler::button_press button_input = input_handler->button_check();

  switch (button_input){
    case InputHandler::left_button_press: // volume down
      audio_settings->volume_down();
      break;
    case InputHandler::left_button_long_press: // previous song
      playlist->PlayPreviousSong();
      break;
    case InputHandler::middle_button_press: // pause/play song
      if (playlist->IsPaused()){
        playlist->Play();
        audio_settings->restore_audio();
      }
      else{
        playlist->Stop();
        audio_settings->shut_audio();
      }
      break;
    case InputHandler::right_button_press: // volume up
      audio_settings->volume_up();
      break;
    case InputHandler::right_button_long_press: // next song
      playlist->PlayNextSong();
      break;
    default:
      break;  
  }
  playlist->PlaylistLoop();
}
