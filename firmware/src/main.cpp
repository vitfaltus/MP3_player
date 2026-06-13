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
  if (filesys_setup()){
    Serial.println("Filesystem set up");
  }
  else {
    Serial.println("Filesystem failed to set up");
  }
  
  Serial.println("Creating components");

  input_handler = new InputHandler();
  audio_settings = new AudioSettings();


  Serial.println("Creating playlist");
  playlist = new Playlist("/");

  Serial.println("Systems set up");

  input_handler->buttons_calibration();
  Serial.println("Buttons calibrated");

}

void loop() {

  InputHandler::button_press button_input = input_handler->button_check();

  if (button_input !=  InputHandler::button_press::none){

    Serial.println(button_input);
  }
  switch (button_input){
    case InputHandler::left_button_press: // volume down
      audio_settings->volume_down();
      break;
    case InputHandler::left_button_long_press: // previous song
      playlist->PlayPreviousSong(audio_settings);
      break;
    case InputHandler::middle_button_press: // pause/play song
      if (playlist->IsPaused()){
        playlist->Play(audio_settings);
        audio_settings->restore_audio();
        Serial.println("audio restored");
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
      playlist->PlayNextSong(audio_settings);
      break;
    default:
      break;  
  }
  playlist->PlaylistLoop(audio_settings);
}
