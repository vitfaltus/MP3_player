#include <Arduino.h>
#include"input_hander.hpp"

InputHandler* input_handler;


void setup() {
  input_handler = new InputHandler();

}

void loop() {


  
  InputHandler::button_press button_input = input_handler->button_check();

  switch (button_input){
    case InputHandler::left_button_press:
      // volume down
      break;
    case InputHandler::left_button_long_press:
      // previous song
      break;
    case InputHandler::middle_button_press:
      // pause/play song
      break;
    case InputHandler::right_button_press:
      // volume up
      break;
    case InputHandler::right_button_long_press:
      // next song
    default:
      break;  
  }
}
