#include <Arduino.h>
#include "pin_config.hpp"

#define ERROR_MARGIN_ANALOG_READ 100


class InputHandler {

    const int no_press_value = 940;

    const int left_analog_value = 1060;
    const int middle_analog_value = 1200;
    const int right_analog_value = 1800;

    const int long_press_treshold = 10000;
    const int short_press_treshold = 50;

    char debounce_timer_left = 0;
    char debounce_timer_middle = 0;
    char debounce_timer_right = 0;

    char debounce_timer_treshhold = 10;

    bool around_value(int reference_value, int compared_value){
        if (std::abs(reference_value - compared_value) < ERROR_MARGIN_ANALOG_READ){
        return true;
    }
    return false;
    }

  public:
    enum button_press {
        none,
        left_button_press,
        middle_button_press,
        right_button_press,
        left_button_long_press,
        middle_button_long_press,
        right_button_long_press
    };

    button_press button_check(){
      
      const int div_cons = 16;
      int button_panel_value = 0;

      for (int i = 0; i < div_cons; i++){
        button_panel_value += analogRead(PinConfig::BUTTON_PANEL);
      }
      button_panel_value /= div_cons;
      

      if (around_value(no_press_value, button_panel_value)){        
        if (debounce_timer_left > debounce_timer_treshhold){
          debounce_timer_left = 0;
          return left_button_press;
        }
        if ( debounce_timer_middle > debounce_timer_treshhold ){
          debounce_timer_middle = 0;
          return middle_button_press;
        }
        if ( debounce_timer_right > debounce_timer_treshhold ){
          debounce_timer_right = 0;
          return right_button_press;
        }
      }
      if (around_value(left_analog_value, button_panel_value)){
        
        debounce_timer_left++;
      }
      else if (around_value(middle_analog_value, button_panel_value) ){
        
        debounce_timer_middle++;
      }
      else if (button_panel_value > 1600){
        debounce_timer_right++;
      }
      else{
        debounce_timer_left = 0;
        debounce_timer_middle = 0;
        debounce_timer_right = 0;
      }
      return none;
    }
};