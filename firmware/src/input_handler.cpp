#include "input_handler.hpp"
#include "pin_config.hpp"
#include <Arduino.h>

bool InputHandler::around_value(int reference_value, int compared_value) {
  if (std::abs(reference_value - compared_value) < ERROR_MARGIN_ANALOG_READ) {
    return true;
  }
  return false;
}

InputHandler::InputHandler() { pinMode(PinConfig::BUTTON_PANEL, INPUT); }

InputHandler::button_press InputHandler::button_check() {

  get_button_panel_value();

  // Serial.println(button_panel_value);

  if (around_value(no_press_value,
                   button_panel_value)) { // button possibly released
    if (debounce_timer_left > short_press_treshold) {
      return handle_left_release();
    }
    if (debounce_timer_middle > short_press_treshold) {
      return handle_middle_release();
    }
    if (debounce_timer_right > short_press_treshold) {
      return handle_right_release();
    }
    clear_debounces();
  }
  if (around_value(left_analog_value, button_panel_value)) {

    debounce_timer_left++;
  } else if (around_value(middle_analog_value, button_panel_value)) {

    debounce_timer_middle++;
  } else if (around_value(right_analog_value, button_panel_value)) {
    debounce_timer_right++;
  }

  return none;
}
void InputHandler::buttons_calibration() {}

InputHandler::button_press InputHandler::handle_left_release() {

  if (debounce_timer_left > long_press_treshold) {
    clear_debounces();
    return left_button_long_press;
  }

  clear_debounces();
  return left_button_press;
}

InputHandler::button_press InputHandler::handle_middle_release() {
  if (debounce_timer_middle > long_press_treshold) {
    clear_debounces();
    return middle_button_long_press;
  }
  clear_debounces();
  return middle_button_press;
}

InputHandler::button_press InputHandler::handle_right_release() {
  if (debounce_timer_right > long_press_treshold) {
    clear_debounces();
    return right_button_long_press;
  }
  clear_debounces();
  return right_button_press;
}

void InputHandler::get_button_panel_value() {
  button_panel_value = 0;

  for (int i = 0; i < div_cons; i++) {
    button_panel_value += analogRead(PinConfig::BUTTON_PANEL);
  }
  button_panel_value /= div_cons;

  // Serial.print(debounce_timer_left);
  // Serial.print(", ");
  // Serial.print(debounce_timer_middle);
  // Serial.print(", ");
  // Serial.print(debounce_timer_right);
  // Serial.print(" ; ");
  // Serial.println(button_panel_value);
}

void InputHandler::clear_debounces() {
  debounce_timer_left = 0;
  debounce_timer_middle = 0;
  debounce_timer_right = 0;
}
