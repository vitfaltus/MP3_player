#include <Arduino.h>
#include "input_hander.hpp"
#include "audio_settings.hpp"

InputHandler* input_handler;
AudioSettings* audio_settings;

void setup() {
  Serial.begin(9600);

  input_handler = new InputHandler();
  audio_settings = new AudioSettings();

}

void loop() {

  InputHandler::button_press button_input = input_handler->button_check();

  switch (button_input){
    case InputHandler::left_button_press: // volume down
      audio_settings->volume_down();
      break;
    case InputHandler::left_button_long_press:
      // previous song
      break;
    case InputHandler::middle_button_press:
      // pause/play song
      break;
    case InputHandler::right_button_press: // volume up
      audio_settings->volume_up();
      break;
    case InputHandler::right_button_long_press:
      // next song
    default:
      break;  
  }
}
