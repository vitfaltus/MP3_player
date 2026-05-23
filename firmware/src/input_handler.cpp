#include <Arduino.h>
#include "pin_config.hpp"
#include "input_handler.hpp"

#define ERROR_MARGIN_ANALOG_READ 60

    

    bool InputHandler::around_value(int reference_value, int compared_value){
        if (std::abs(reference_value - compared_value) < ERROR_MARGIN_ANALOG_READ){
        return true;
    }
    return false;
    }


    InputHandler::InputHandler(){
      pinMode(PinConfig::BUTTON_PANEL, INPUT);
    }


    InputHandler::button_press InputHandler::button_check(){
      

      for (int i = 0; i < div_cons; i++){
        button_panel_value += analogRead(PinConfig::BUTTON_PANEL);
      }
      button_panel_value /= div_cons;
      
      //Serial.println(button_panel_value);

      if (InputHandler::around_value(no_press_value, button_panel_value)){ // button possibly released    
        if (debounce_timer_left > debounce_timer_treshhold){
          Serial.println(debounce_timer_left);
          return handle_left_release();
        }
        if ( debounce_timer_middle > debounce_timer_treshhold ){
          return handle_middle_release();
        }
        if ( debounce_timer_right > debounce_timer_treshhold ){
          return handle_right_release();
        }
        else{
          clear_debounces();
        }
      }
      if (around_value(left_analog_value, button_panel_value)){
        
        debounce_timer_left++;
      }
      else if (around_value(middle_analog_value, button_panel_value) ){
        
        debounce_timer_middle++;
      }
      else if (around_value(right_analog_value, button_panel_value)){
        debounce_timer_right++;
      }
      else{
        clear_debounces();
      }
      return InputHandler::button_press::none;
    }


InputHandler::button_press InputHandler::handle_left_release(){
  
  if (debounce_timer_left > long_press_treshold){
    debounce_timer_left = 0;
    return InputHandler::button_press::left_button_long_press;
  }
  debounce_timer_left = 0;
  return InputHandler::button_press::left_button_press;
}

InputHandler::button_press InputHandler::handle_middle_release(){
  if (debounce_timer_middle > long_press_treshold){
    debounce_timer_middle = 0;
    return InputHandler::button_press::middle_button_long_press;
  }
  debounce_timer_middle = 0;
  return InputHandler::button_press::middle_button_press;
}

InputHandler::button_press InputHandler::handle_right_release(){
  if (debounce_timer_right > long_press_treshold){
    debounce_timer_right = 0;
    return InputHandler::button_press::right_button_long_press;
  }
  debounce_timer_right = 0;
  return InputHandler::button_press::right_button_press;
}

void InputHandler::clear_debounces(){
  debounce_timer_left = 0;
  debounce_timer_middle = 0;
  debounce_timer_right = 0;
}

