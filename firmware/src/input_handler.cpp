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

      if (InputHandler::around_value(no_press_value, button_panel_value)){        
        if (debounce_timer_left > debounce_timer_treshhold){
          debounce_timer_left = 0;
          return InputHandler::button_press::left_button_press;
        }
        if ( debounce_timer_middle > debounce_timer_treshhold ){
          debounce_timer_middle = 0;
          return InputHandler::button_press::middle_button_press;
        }
        if ( debounce_timer_right > debounce_timer_treshhold ){
          debounce_timer_right = 0;
          return InputHandler::button_press::right_button_press;
        }
        else{
          debounce_timer_left = 0;
          debounce_timer_middle = 0;
          debounce_timer_right = 0;
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
        debounce_timer_left = 0;
        debounce_timer_middle = 0;
        debounce_timer_right = 0;
      }
      return InputHandler::button_press::none;
    }